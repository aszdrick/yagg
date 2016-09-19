/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include "gomoku/RangeBoard.hpp"
#include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

void RangeBoard::play(const go::Position& position, go::Team team) {
    // ECHO("[PLAY]");
    // std::cout << "position: " << position.row << " " << position.column << std::endl;
    auto start = std::chrono::system_clock::now().time_since_epoch();
    unsigned short row = position.row;
    unsigned short column = position.column;

    assert(position.row < 15 && position.column < 15);

    stones.push_back({position, team});

    for (auto i = 0; i < 4; i++) {
        auto index = mappers[i](row, column);
        if (index > -1) {
            auto iv = choosers[i](row, column);
            // TRACE(iv);
            if (!lines[i][index].count(iv)) {
                // ECHO("create");
                create(lines[i][index], iv, team);
                sanity.push("create");
            } else {
                solve(lines[i][index], iv, team);
            }
            // std::cout << "--------------- MAP " << i << " -----------------" << std::endl;
            // for (auto interval : lines[i][index]) {
                // TRACE(interval.first);
                // TRACE_IT(sequences[interval.second].placedPositions);
            // }
        }
    }
    // ECHO("[END PLAY]");
    auto end = std::chrono::system_clock::now().time_since_epoch();
    auto play_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (play_delay > 0) {
        // TRACE(play_delay);
    }
}

void RangeBoard::undo() {
    // ECHO("[UNDO]");
    auto start = std::chrono::system_clock::now().time_since_epoch();
    if (!stones.empty()) {
        auto& stone = stones.back();
        auto& position = stone.position;
        for (auto i = 3; i > -1; i--) {
            // std::cout << "------------------ " << i << " ------------------" << std::endl;
            auto index = mappers[i](position.row, position.column);
            auto& map = lines[i][index];
            if (index > -1) {
                // TRACE(position.row);
                // TRACE(position.column);
                auto desired = choosers[i](position.row, position.column);
                // TRACE(desired);
                auto atom = Interval::unitary(desired.center_low);
                // TRACE(atom);
                assert(map.count(atom));
                auto it = map.find(atom);
                auto iv = it->first;
                auto size = iv.size();
                // TRACE(iv);
                // TRACE(size);
                // TRACE(desired.size());
                auto distance = iv.center_distance(atom);
                // TRACE(distance);
                if (distance >= -1) {
                    if (iv.center_low != iv.center_high) {
                        // TRACE(sanity.top());
                        assert(sanity.top() == "increase");
                        sanity.pop();
                        undoSideMove(map, it, iv, atom, desired);
                    } else if (size == desired.size()) {
                        // SEQUENCE ONLY CREATED
                        // TRACE(sanity.top());
                        assert(sanity.top() == "create");
                        sanity.pop();
                        undoCreate(map, it);
                    } else if (size < 4 
                               && it != map.begin() 
                               && std::next(it) != map.end()) {
                        // SEQUENCE SPLITED
                        // TRACE(sanity.top());
                        assert(sanity.top() == "split");
                        sanity.pop();
                        undoSplit(map, it);
                    } else {
                        // SEQUENCE(S) RESIZED
                        // TRACE(sanity.top());
                        // TRACE(sanity.size());
                        assert(sanity.top() == "resize");
                        sanity.pop();
                        undoResize(map, it, iv, atom);
                    }
                } else {
                    // ECHO("CENTRAL MOVE");
                    // STONE PLACED IN CENTER
                    undoCentralMove(map, it, atom);
                }
            }
            // std::cout << "--------------- MAP " << i << " -----------------" << std::endl;
            // for (auto interval : map) {
                // TRACE(interval.first);
            // }
        }
        stones.pop_back();
    }
    // ECHO("[END UNDO]");
    auto end = std::chrono::system_clock::now().time_since_epoch();
    auto undo_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (undo_delay > 0) {
        // TRACE(undo_delay);
    }
}

void RangeBoard::solve(IvMap& map, Interval iv, go::Team team) {
    static constexpr auto mergeDistance = 5;
    auto mergeCount = 0;
    auto merges = std::array<assoc, 2>();
    auto noMerges = true;
    auto it = map.find(iv);

    while (it != map.end()) {
        auto interval = it->first;
        if (interval.low > iv.center_low || interval.high < iv.center_high) {
            it = std::next(it);
            continue;
        }
        // TRACE(interval);
        auto diff = interval.center_distance(iv);
        if (sequences[it->second].team == team && diff < mergeDistance) {
            merges[mergeCount] = premerge(map, it);
            noMerges = false;
            ++mergeCount;
        } else if (!resize(map, it, iv)) {
            sanity.push("split");
            // ECHO("split");
            split(map, it, iv);
            return;
        }
        it = map.find(iv);
    }
    
    if (noMerges) {
        // ECHO("resize");
        sanity.push("resize");
        create(map, iv, team);
    } else if (mergeCount > 1) {
        // ECHO("merge");
        sanity.push("merge");
        merge(map, merges, iv, team);
    } else {
        // ECHO("increase");
        sanity.push("increase");
        increase(map, merges[0], iv, team);
    }
}

assoc RangeBoard::premerge(IvMap& map, const IvMap::iterator& it) {
    auto pair = std::make_pair(it->first, it->second);
    map.erase(it);
    return pair;
}

void RangeBoard::merge(IvMap& map, std::array<assoc,2>& merges,
                       Interval& iv, go::Team t) {
    auto low = 0, high = 1;

    if (merges[1].first < merges[0].first) {
        low = 1;
        high = 0;
    }

    merge_keys.push(merges[high].second);

    auto& low_piv = merges[low].first;
    auto& high_piv = merges[high].first;
    auto& low_sequence = sequences[merges[low].second];
    auto& high_sequence = sequences[merges[high].second];
    auto& low_positions = low_sequence.placedPositions;
    auto& high_positions = high_sequence.placedPositions;

    low_positions.insert(iv.center_low);

    std::set_union(
        low_positions.begin(), low_positions.end(),
        high_positions.begin(), high_positions.end(),
        std::inserter(low_positions, low_positions.end())
    );

    low_sequence.sequential = high_piv.low - low_piv.high == 1
        && low_sequence.sequential
        && high_sequence.sequential;

    low_piv.high = high_piv.high;
    low_piv.center_high = high_piv.center_high;

    low_sequence.totalSize += high_sequence.totalSize + 1;
    low_sequence.capacity = low_piv.size();
    low_sequence.openings.second = high_sequence.openings.second;

    sequences.erase(merges[high].second);
    map.insert(std::move(merges[low]));

    auto old = ended;
    ended = old || (low_sequence.sequential && low_sequence.totalSize > 4);
}

void RangeBoard::increase(IvMap& map, assoc& pair, Interval& iv, go::Team t) {
    auto& piv = pair.first;
    auto& sequence = sequences[pair.second];
    auto distance = piv.center_distance(iv);
    auto team_key = static_cast<unsigned short>(t);
    auto increment = 0;

    // TRACE_IT(sequence.placedPositions);
    sequence.placedPositions.insert(iv.center_low);
    // TRACE_IT(sequence.placedPositions);

    if (distance >= 0) {
        sequence.sequential = sequence.sequential && distance == 0;
        if (piv.center_low > iv.center_high) {
            increment = piv.low - iv.low;
            piv.low = iv.low;
            piv.center_low = iv.center_low;
            sequence.openings.first = (iv.center_low - iv.low) > 0;
        } else {
            increment = piv.high - iv.high;
            piv.high = iv.high;
            piv.center_high = iv.center_high;
            sequence.openings.second = (iv.high - iv.center_high) > 0;
        }
    } else {
        sequence.updateSequentiality();
    }
    
    ++sequence.totalSize;
    sequence.capacity = piv.size();
    map.insert(std::move(pair));
    auto old = ended;
    ended = old || (sequence.sequential && sequence.totalSize > 4);
    dominations[team_key] += increment;

}

void RangeBoard::split(IvMap& map, const IvMap::iterator& it, Interval& iv) {
    auto upper_key = it->second;
    auto lower_key = currentSequence++;
    auto& sequence = sequences[upper_key];
    auto original_iv = it->first;
    auto upper_iv = original_iv;
    auto begin = sequence.placedPositions.begin();
    auto upper = sequence.placedPositions.upper_bound(iv.center_low);
    assert(upper != sequence.placedPositions.end());
    auto lower = std::prev(upper);
    auto low = upper_iv.low;
    auto team_value = static_cast<unsigned short>(sequence.team);

    iv.low = *lower + 1;
    iv.high = *upper - 1;

    upper_iv.low = iv.center_high + 1;
    upper_iv.center_low = *upper;

    auto lower_iv = Interval{low, *begin, *lower,
        static_cast<unsigned short>(iv.center_low - 1)};
    auto capacity = lower_iv.size();
    auto lowerPositions = std::set<unsigned short>(begin, upper);
    // TRACE_IT(lowerPositions);
    auto lowerOpening = sequence.openings.first;
    
    sequence.placedPositions.erase(begin, upper);
    // TRACE_IT(sequence.placedPositions);
    sequence.totalSize = sequence.placedPositions.size();
    sequence.capacity -= capacity;
    sequence.openings.first = (upper_iv.center_low - iv.center_high) > 1;
    sequence.updateSequentiality();

    sequences[lower_key] = {
        sequence.team,
        static_cast<unsigned short>(lowerPositions.size()),
        capacity,
        true,
        {lowerOpening, (iv.center_low - lower_iv.center_high) > 1},
        lowerPositions
    };

    sequences[lower_key].updateSequentiality();

    auto hint = map.erase(it);
    hint = map.insert(hint, {lower_iv, lower_key});
    map.insert(hint, {upper_iv, upper_key});

    --dominations[team_value];

    create(map, iv, static_cast<go::Team>(1 - team_value));
}

bool RangeBoard::resize(IvMap& map, const IvMap::iterator& it, Interval& iv) {
    auto original = it->first;
    auto size = original.size();
    auto resized = original;
    auto& sequence = sequences[it->second];
    auto enemy = static_cast<unsigned short>(sequence.team);

    if (iv.center_low > resized.center_low 
        && iv.center_high < resized.center_high) {
        return false;
    }

    if (resized.center_low > iv.center_high) {
        resized.low = iv.center_high + 1;
        iv.high = resized.center_low - 1;
    } else {
        resized.high = iv.center_low - 1;
        iv.low = resized.center_high + 1;
    }

    auto hint = map.erase(it);
    // TRACE(resized);
    map.insert(hint, {resized, it->second});

    auto decrement = size - resized.size();
    dominations[enemy] -= decrement;
    sequence.capacity -= decrement;

    return true;
}

void RangeBoard::create(IvMap& map, Interval& iv, go::Team t) {
    auto key = currentSequence;
    sequences[currentSequence] = {
        t,              // Team
        1,              // Total size
        iv.size(),      // Sequence capacity
        true,           // Sequential
        {true, true},   // Openings
        {iv.center_low} // Placed positions
    };

    map.insert({iv, key});
    auto increment = iv.size();
    auto team_key = static_cast<unsigned short>(t);
    dominations[team_key] += increment;
    ++currentSequence;
}

void RangeBoard::undoCreate(IvMap& map, const IvMap::iterator& it) {
    // ECHO("undo create");
    auto piv = it->first;
    auto key = it->second;
    auto seq_it = sequences.find(key);
    auto team = seq_it->second.team;

    sequences.erase(seq_it);
    map.erase(it);

    dominations[static_cast<unsigned short>(team)] -= piv.size();
    --currentSequence;
}

void RangeBoard::undoCentralMove(IvMap& map,
                                 const IvMap::iterator& it,
                                 const Interval& atom) {
    auto key = it->second;
    auto& sequence = sequences[key];
    auto seq_it = sequence.placedPositions.find(atom.center_low);
    auto lower_it = std::prev(seq_it);
    auto upper_it = std::next(seq_it);

    assert(seq_it != sequence.placedPositions.begin());
    assert(upper_it != sequence.placedPositions.end());

    if (*upper_it - *lower_it > 4) {
        // TRACE(sanity.top());
        assert(sanity.top() == "merge");
        sanity.pop();
        undoMerge(map, it, atom);
    } else {
        // TRACE(sanity.top());
        assert(sanity.top() == "increase");
        sanity.pop();
        sequence.placedPositions.erase(seq_it);
        sequence.updateSequentiality();
        --sequence.totalSize;
    }
}

void RangeBoard::undoMerge(IvMap& map,
                           const IvMap::iterator& it,
                           const Interval& atom) {
    // ECHO("undo merge");
    auto lower_iv = it->first;
    auto upper_iv = lower_iv;
    auto key = it->second;
    auto& upper_sequence = sequences[key];
    auto seq_it = upper_sequence.placedPositions.find(atom.center_low);
    auto lower_it = std::prev(seq_it);
    auto upper_it = std::next(seq_it);
    auto begin = upper_sequence.placedPositions.begin();
    auto old_key = merge_keys.top();
    merge_keys.pop();

    upper_iv.center_low = *upper_it;
    lower_iv.center_high = *lower_it;

    upper_iv.low = lower_iv.center_high + 1;
    lower_iv.high = upper_iv.center_low - 1;

    auto positions = std::set<unsigned short>(begin, seq_it);

    upper_sequence.placedPositions.erase(begin, upper_it);
    upper_sequence.totalSize = upper_sequence.placedPositions.size();
    upper_sequence.capacity = upper_iv.size();

    sequences[old_key] = {
        upper_sequence.team,
        static_cast<unsigned short>(positions.size()),
        lower_iv.size(),
        true,
        {upper_sequence.openings.first, true},
        positions
    };

    sequences[old_key].updateSequentiality();
    upper_sequence.openings.first = true;

    // TRACE(lower_iv);
    // TRACE(upper_iv);

    auto hint = map.erase(it);
    hint = map.insert(hint, {lower_iv, old_key});
    map.insert(hint, {upper_iv, key});
}

void RangeBoard::undoSideMove(IvMap& map, const IvMap::iterator& it,
                              const Interval& iv, const Interval& atom,
                              const Interval& desired) {
    auto upper_it = std::next(it);
    auto lower_it = std::prev(it);
    // SEQUENCE INCREASED
    auto original = undoIncrease(map, it, atom);

    if (iv.center_low == atom.center_low) {
        if (iv.low > desired.low) {
            assert(it != map.begin());
            // LOWER SEQUENCE RESIZED
            auto piv = lower_it->first;
            auto old = piv.high;
            piv.high = std::min(piv.center_high + 4, original.center_low - 1);
            undoResize(map, lower_it, piv, piv.high - old);
        }
    } else if (iv.center_high == atom.center_high) {
        if (iv.high < desired.high) {
            assert(upper_it != map.end());
            // UPPER SEQUENCE RESIZED
            auto piv = upper_it->first;
            auto old = piv.low;
            piv.low = std::max(piv.center_low - 4, original.center_high + 1);
            undoResize(map, upper_it, piv, old - piv.low);
        }
    }
}

void RangeBoard::undoResize(IvMap& map, const IvMap::iterator& it,
                            const Interval& iv, const Interval& atom) {
    // ECHO("undo resize");
    auto lower_diff = iv.center_low - iv.low;
    auto upper_diff = iv.high - iv.center_high;
    auto sanityCheck = false;
    auto upper_it = std::next(it);
    auto lower_it = std::prev(it);
    auto has_lower = it != map.begin();
    auto has_upper = upper_it != map.end();

    undoCreate(map, it);

    if (lower_diff < 4 && has_lower) {
        // ECHO("lower resize");
        auto piv = lower_it->first;
        auto diff = 4 - (piv.high - piv.center_high);
        // TRACE(piv);
        // TRACE(diff);
        // TRACE(upper_diff);
        auto increment = std::min(diff, upper_diff + 1);
        piv.high += increment;
        undoResize(map, lower_it, piv, increment);
        sanityCheck = true;
    }

    if (upper_diff < 4 && has_upper) {
        // ECHO("upper resize");
        auto piv = upper_it->first;
        auto diff = 4 - (piv.center_low - piv.low);
        auto increment = std::min(diff, lower_diff + 1);
        piv.low -= increment;
        undoResize(map, upper_it, piv, increment);
        sanityCheck = true;
    }

    assert(sanityCheck || !(has_lower || has_upper));
}

void RangeBoard::undoResize(IvMap& map,
                            const IvMap::iterator& it,
                            const Interval& original,
                            unsigned short increment) {
    // ECHO("undo resize [inner]");
    auto key = it->second;
    auto& sequence = sequences[key];
    auto team = static_cast<unsigned short>(sequence.team);
    // TRACE(original);
    assert(original.low <= original.center_low && original.center_low <= original.center_high && original.center_high <= original.high);
    auto hint = map.erase(it);
    map.insert(hint, {original, key});
    
    dominations[team] += increment;
    sequence.capacity += increment;
}

Interval RangeBoard::undoIncrease(IvMap& map,
                              const IvMap::iterator& it,
                              const Interval& atom) {
    // ECHO("undo increase");
    auto original = it->first;
    auto key = it->second;
    auto& sequence = sequences[key];
    auto team = static_cast<unsigned short>(sequence.team);
    auto begin = sequence.placedPositions.begin();
    auto end = std::prev(sequence.placedPositions.end());
    auto decrement = 0;

    if (atom.center_low == *begin) {
        auto next = std::next(begin);
        sequence.placedPositions.erase(begin);
        original.center_low = *next;
        int old = original.low;
        // assert((original.center_low - 4) < 15);
        original.low = std::max(old, original.center_low - 4);
        decrement = original.low - old;
    } else {
        assert(atom.center_low == *end);
        auto prev = std::prev(end);
        sequence.placedPositions.erase(end);
        original.center_high = *prev;
        int old = original.high;
        // assert((original.center_high + 4) < 15);
        original.high = std::min(old, original.center_high + 4);
        decrement = old - original.high;
    }

    auto hint = map.erase(it);
    // TRACE(original);
    assert(original.low <= original.center_low && original.center_low <= original.center_high && original.center_high <= original.high);
    map.insert(hint, {original, key});
    
    --sequence.totalSize;
    dominations[team] -= decrement;

    return original;
}

void RangeBoard::undoSplit(IvMap& map, const IvMap::iterator& it) {
    assert(it != map.begin());

    auto upper_it = std::next(it);
    auto lower_it = std::prev(it);

    assert(upper_it != map.end());

    undoCreate(map, it);

    undoSplit(map, upper_it, lower_it);
}

void RangeBoard::undoSplit(IvMap& map,
                           const IvMap::iterator& upper_it,
                           const IvMap::iterator& lower_it) {
    // ECHO("undo split");
    auto original = upper_it->first;
    auto lower_iv = lower_it->first;
    auto upper_key = upper_it->second;
    auto lower_key = lower_it->second;
    auto& upper_sequence = sequences[upper_key];
    auto& lower_sequence = sequences[lower_key];

    map.erase(upper_it);
    map.erase(lower_it);

    original.low = lower_iv.low;
    original.center_low = lower_iv.center_low;

    std::set_union(
        upper_sequence.placedPositions.begin(),
        upper_sequence.placedPositions.end(),
        lower_sequence.placedPositions.begin(),
        lower_sequence.placedPositions.end(),
        std::inserter(upper_sequence.placedPositions,
            upper_sequence.placedPositions.begin())
    );

    upper_sequence.totalSize += lower_sequence.totalSize;
    upper_sequence.capacity = original.size();
    upper_sequence.updateSequentiality();

    ++dominations[static_cast<unsigned short>(upper_sequence.team)];

    sequences.erase(lower_key);
    // TRACE(original);
    assert(original.low <= original.center_low && original.center_low <= original.center_high && original.center_high <= original.high);
    map.insert({original, upper_key});
    --currentSequence;
}

bool RangeBoard::occupied(const go::Position& position) const {
    return false;
}

bool Sequence::updateSequentiality() {
    auto fragmented = false;
    auto prev = placedPositions.begin();
    auto curr = std::next(prev);
    while(curr != placedPositions.end()) {
        if (*curr - *prev != 1) {
            fragmented = true;
            break;
        }
        prev = curr;
        curr = std::next(prev);
    }
    sequential = !fragmented;
    return sequential;
}

namespace {
    bool validateIndex(unsigned short index) {
        static constexpr auto limit = GomokuTraits::BOARD_DIMENSION;
        static constexpr auto size = GomokuTraits::WIN_CONDITION;
        static constexpr auto lower_threshold = size - 1;
        static constexpr auto upper_threshold = 2 * limit - size - 1;
        return index >= lower_threshold && index <= upper_threshold;
    }
}

const std::array<IndexMapper, 4> RangeBoard::mappers = {
    [](unsigned short row, unsigned short column) {
        return row;
    },
    [](unsigned short row, unsigned short column) {
        return column;
    },
    [](unsigned short row, unsigned short column) {
        auto index = GomokuTraits::BOARD_DIMENSION + row - column;
        if (!validateIndex(index)) {
            index = -1;
        }
        return index;
    },
    [](unsigned short row, unsigned short column) {
        auto index = row + column;
        if (!validateIndex(index)) {
            index = -1;
        }
        return index;
    }
};

const std::array<RangeChooser, 4> RangeBoard::choosers = {
    [](unsigned short row, unsigned short column) {
        static constexpr short limit = GomokuTraits::BOARD_DIMENSION;
        auto iv = Interval{
            static_cast<unsigned short>(std::max(column - 4, 0)),
            column, column,
            static_cast<unsigned short>(std::min(column + 4, limit - 1))
        };
        return iv;
    },
    [](unsigned short row, unsigned short column) {
        static constexpr short limit = GomokuTraits::BOARD_DIMENSION;
        auto iv = Interval{
        static_cast<unsigned short>(std::max(row - 4, 0)),
        row, row,
        static_cast<unsigned short>(std::min(row + 4, limit - 1))
        };
        return iv;
    },
    [](unsigned short row, unsigned short column) {
        static constexpr short limit = GomokuTraits::BOARD_DIMENSION;
        auto iv = Interval{
        static_cast<unsigned short>(std::max(row - 4, 0)),
        row, row,
        static_cast<unsigned short>(std::min(row + 4, limit - 1))
        };
        return iv;
    },
    [](unsigned short row, unsigned short column) {
        static constexpr short limit = GomokuTraits::BOARD_DIMENSION;
        auto iv = Interval{
        static_cast<unsigned short>(std::max(row - 4, 0)),
        row, row,
        static_cast<unsigned short>(std::min(row + 4, limit - 1))
        };
        return iv;
    }  
};
