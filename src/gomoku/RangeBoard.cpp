/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cassert>
#include <chrono>
#include "gomoku/RangeBoard.hpp"
#include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

void RangeBoard::iterate(const StoneCallback& fn) const {
    for (auto& stone : stones) {
        fn(stone);
    }
}

void RangeBoard::iterateCriticalZone(const PositionCallback& fn) const {
    for (auto i = 0; i < 4; i++) {
        for (auto entry : lines[i]) {
            for (auto pair : entry.second) {
                auto& sequence = sequences.at(pair.second);
                if (sequence.fragmentation == 0 && sequence.totalSize == 4) {
                    if (sequence.openings.first) {
                        auto begin = *sequence.positions.begin();
                        --begin;
                        fn(unmappers[i](entry.first, begin));
                    }
                    if (sequence.openings.second) {
                        auto end = *std::prev(sequence.positions.end());
                        ++end;
                        fn(unmappers[i](entry.first, end));
                    }
                } else if (sequence.totalSize >= 4) {
                    auto holes = sequence.findCriticalHoles();
                    for (auto hole : holes) {
                        fn(unmappers[i](entry.first, hole));
                    }
                }
            }
        }
    }
}

void RangeBoard::classify(unsigned short key, const Sequence& sequence,
                          const Interval& iv) {
    auto team = static_cast<unsigned short>(sequence.team);
    auto size = sequence.totalSize;
    auto frag = sequence.fragmentation;
    auto open = !!sequence.openings.first + !!sequence.openings.second;
    classified_sequences[team][size][frag][open].insert(key);

    ++sequencesCount;

    if (iv.size() < 5) {
        ++deadSequences;
    }
}

void RangeBoard::unclassify(unsigned short key, const Sequence& sequence,
                            const Interval& iv) {
    auto team = static_cast<unsigned short>(sequence.team);
    auto size = sequence.totalSize;
    auto frag = sequence.fragmentation;
    auto open = !!sequence.openings.first + !!sequence.openings.second;
    classified_sequences[team][size][frag][open].erase(key);

    --sequencesCount;

    if (iv.size() < 5) {
        --deadSequences;
    }
}

void RangeBoard::play(const go::Position& position, go::Team team) {
    assert(position.row < 15 && position.column < 15);

    stones.push_back({position, team});
    placed_positions.insert(position);

    search_space.play(position, *this);

    for (auto i = 0; i < 4; i++) {
        auto index = mappers[i](position.row, position.column);
        if (index > -1) {
            auto iv = choosers[i](position.row, position.column);
            if (!lines[i][index].count(iv)) {
                create(lines[i][index], iv, team);
                sanity.push("create");
            } else {
                solve(lines[i][index], iv, team);
            }
        }
    }
}

void RangeBoard::undo() {
    if (!stones.empty()) {
        auto& stone = stones.back();
        auto& position = stone.position;
        placed_positions.erase(position);
        search_space.undo(position, *this);
        for (auto i = 3; i > -1; i--) {
            auto index = mappers[i](position.row, position.column);
            auto& map = lines[i][index];
            if (index > -1) {
                auto desired = choosers[i](position.row, position.column);
                auto atom = Interval::unitary(desired.center_low);
                assert(map.count(atom));
                auto it = map.find(atom);
                auto iv = it->first;
                auto size = iv.size();
                auto distance = iv.center_distance(atom);
                if (distance >= -1) {
                    if (iv.center_low != iv.center_high) {
                        assert(sanity.top() == "increase");
                        sanity.pop();
                        undoSideMove(map, it, iv, atom, desired);
                    } else if (size == desired.size()) {
                        assert(sanity.top() == "create");
                        sanity.pop();
                        undoCreate(map, it);
                    } else if (size < 4 
                               && it != map.begin() 
                               && std::next(it) != map.end()) {
                        assert(sanity.top() == "split");
                        sanity.pop();
                        undoSplit(map, it);
                    } else {
                        TRACE(iv);
                        TRACE(sanity.top());
                        assert(sanity.top() == "resize");
                        sanity.pop();
                        undoResize(map, it, iv, atom);
                    }
                } else {
                    undoCentralMove(map, it, atom);
                }
            }
        }
        stones.pop_back();
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
        auto diff = interval.center_distance(iv);
        if (sequences[it->second].team == team && diff < mergeDistance) {
            merges[mergeCount] = premerge(map, it);
            noMerges = false;
            ++mergeCount;
        } else if (!resize(map, it, iv)) {
            sanity.push("split");
            split(map, it, iv);
            return;
        }
        it = map.find(iv);
    }
    
    if (noMerges) {
        sanity.push("resize");
        create(map, iv, team);
    } else if (mergeCount > 1) {
        sanity.push("merge");
        merge(map, merges, iv);
    } else {
        sanity.push("increase");
        increase(map, merges[0], iv);
    }
}

RangeBoard::assoc RangeBoard::premerge(IvMap& map, const IvMap::iterator& it) {
    auto pair = std::make_pair(it->first, it->second);
    map.erase(it);
    return pair;
}

void RangeBoard::merge(IvMap& map, std::array<assoc,2>& merges, Interval& iv) {
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
    auto& low_positions = low_sequence.positions;
    auto& high_positions = high_sequence.positions;

    unclassify(merges[low].second, low_sequence, merges[low].first);
    unclassify(merges[high].second, high_sequence, merges[high].first);

    low_positions.insert(iv.center_low);

    std::set_union(
        low_positions.begin(), low_positions.end(),
        high_positions.begin(), high_positions.end(),
        std::inserter(low_positions, low_positions.end())
    );

    low_sequence.updateSequentiality();

    low_piv.high = high_piv.high;
    low_piv.center_high = high_piv.center_high;

    low_sequence.totalSize += high_sequence.totalSize + 1;
    low_sequence.capacity = low_piv.size();
    low_sequence.openings.second = high_sequence.openings.second;

    sequences.erase(merges[high].second);
    map.insert(std::move(merges[low]));

    classify(merges[low].second, low_sequence, merges[low].first);

    ended_values.push(ended);
    ended = ended || low_sequence.checkWinCondition();
}

void RangeBoard::increase(IvMap& map, assoc& pair, Interval& iv) {
    auto& piv = pair.first;
    auto& sequence = sequences[pair.second];
    auto distance = piv.center_distance(iv);
    auto team_key = static_cast<unsigned short>(sequence.team);
    auto increment = 0;

    unclassify(pair.second, sequence, piv);

    sequence.positions.insert(iv.center_low);

    if (distance >= 0) {
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
    }
    
    sequence.updateSequentiality();
    
    ++sequence.totalSize;
    sequence.capacity = piv.size();
    map.insert(std::move(pair));

    dominations[team_key] += increment;
    classify(pair.second, sequence, piv);

    ended_values.push(ended);
    ended = ended || sequence.checkWinCondition();
}

void RangeBoard::split(IvMap& map, const IvMap::iterator& it, Interval& iv) {
    auto upper_key = it->second;
    auto lower_key = currentSequence++;
    auto& sequence = sequences[upper_key];
    auto original_iv = it->first;
    auto upper_iv = original_iv;
    auto begin = sequence.positions.begin();
    auto upper = sequence.positions.upper_bound(iv.center_low);
    assert(upper != sequence.positions.end());
    auto lower = std::prev(upper);
    auto low = upper_iv.low;
    auto team_value = static_cast<unsigned short>(sequence.team);

    unclassify(upper_key, sequence, upper_iv);

    iv.low = *lower + 1;
    iv.high = *upper - 1;

    upper_iv.low = iv.center_high + 1;
    upper_iv.center_low = *upper;

    auto lower_iv = Interval{low, *begin, *lower,
        static_cast<unsigned short>(iv.center_low - 1)};
    auto capacity = lower_iv.size();
    auto lowerPositions = std::set<unsigned short>(begin, upper);
    auto lowerOpening = sequence.openings.first;
    
    sequence.positions.erase(begin, upper);
    sequence.totalSize = sequence.positions.size();
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
    classify(lower_key, sequences[lower_key], lower_iv);
    classify(upper_key, sequence, upper_iv);

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
    map.insert(hint, {resized, it->second});

    auto decrement = size - resized.size();
    dominations[enemy] -= decrement;
    sequence.capacity -= decrement;

    return true;
}

void RangeBoard::create(IvMap& map, Interval& iv, go::Team team) {
    auto key = currentSequence;
    sequences[currentSequence] = {
        team,           // Team
        1,              // Total size
        iv.size(),      // Sequence capacity
        true,           // Sequential
        {true, true},   // Openings
        {iv.center_low} // Placed positions
    };

    map.insert({iv, key});
    auto increment = iv.size();
    auto team_key = static_cast<unsigned short>(team);
    dominations[team_key] += increment;
    ++currentSequence;

    classify(key, sequences[key], iv);
}

void RangeBoard::undoCreate(IvMap& map, const IvMap::iterator& it) {
    auto piv = it->first;
    auto key = it->second;
    auto seq_it = sequences.find(key);
    auto team = seq_it->second.team;

    unclassify(key, sequences[key], piv);

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
    auto seq_it = sequence.positions.find(atom.center_low);
    auto lower_it = std::prev(seq_it);
    auto upper_it = std::next(seq_it);

    assert(seq_it != sequence.positions.begin());
    assert(upper_it != sequence.positions.end());

    if (*upper_it - *lower_it > 4) {
        assert(sanity.top() == "merge");
        sanity.pop();
        undoMerge(map, it, atom);
    } else {
        assert(sanity.top() == "increase");
        sanity.pop();

        unclassify(key, sequence, it->first);

        sequence.positions.erase(seq_it);
        sequence.updateSequentiality();
        --sequence.totalSize;

        classify(key, sequence, it->first);

        ended = ended_values.top();
        ended_values.pop();
    }
}

void RangeBoard::undoMerge(IvMap& map, const IvMap::iterator& it,
                           const Interval& atom) {
    auto lower_iv = it->first;
    auto upper_iv = lower_iv;
    auto key = it->second;
    auto& upper_sequence = sequences[key];
    auto seq_it = upper_sequence.positions.find(atom.center_low);
    auto lower_it = std::prev(seq_it);
    auto upper_it = std::next(seq_it);
    auto begin = upper_sequence.positions.begin();
    auto old_key = merge_keys.top();
    merge_keys.pop();

    unclassify(key, upper_sequence, upper_iv);

    upper_iv.center_low = *upper_it;
    lower_iv.center_high = *lower_it;

    upper_iv.low = lower_iv.center_high + 1;
    lower_iv.high = upper_iv.center_low - 1;

    auto positions = std::set<unsigned short>(begin, seq_it);

    upper_sequence.positions.erase(begin, upper_it);
    upper_sequence.totalSize = upper_sequence.positions.size();
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

    auto hint = map.erase(it);
    hint = map.insert(hint, {lower_iv, old_key});
    map.insert(hint, {upper_iv, key});

    classify(key, upper_sequence, upper_iv);
    classify(old_key, sequences[old_key], lower_iv);

    ended = ended_values.top();
    ended_values.pop();
}

void RangeBoard::undoSideMove(IvMap& map, const IvMap::iterator& it,
                              const Interval& iv, const Interval& atom,
                              const Interval& desired) {
    auto upper_it = std::next(it);
    auto lower_it = std::prev(it);
    auto original = undoIncrease(map, it, atom);

    if (iv.center_low == atom.center_low) {
        if (iv.low > desired.low) {
            assert(it != map.begin());
            auto piv = lower_it->first;
            auto old = piv.high;
            piv.high = std::min(piv.center_high + 4, original.center_low - 1);
            undoResize(map, lower_it, piv, piv.high - old);
        }
    } else if (iv.center_high == atom.center_high) {
        if (iv.high < desired.high) {
            assert(upper_it != map.end());
            auto piv = upper_it->first;
            auto old = piv.low;
            piv.low = std::max(piv.center_low - 4, original.center_high + 1);
            undoResize(map, upper_it, piv, old - piv.low);
        }
    }
}

void RangeBoard::undoResize(IvMap& map, const IvMap::iterator& it,
                            const Interval& iv, const Interval& atom) {
    auto lower_diff = iv.center_low - iv.low;
    auto upper_diff = iv.high - iv.center_high;
    auto sanityCheck = false;
    auto upper_it = std::next(it);
    auto lower_it = std::prev(it);
    auto has_lower = it != map.begin();
    auto has_upper = upper_it != map.end();

    undoCreate(map, it);

    if (lower_diff < 4 && has_lower) {
        auto piv = lower_it->first;
        auto diff = 4 - (piv.high - piv.center_high);
        auto increment = std::min(diff, upper_diff + 1);
        piv.high += increment;
        undoResize(map, lower_it, piv, increment);
        sanityCheck = true;
    }

    if (upper_diff < 4 && has_upper) {
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
    auto key = it->second;
    auto& sequence = sequences[key];
    auto team = static_cast<unsigned short>(sequence.team);
    assert(original.low <= original.center_low
        && original.center_low <= original.center_high
        && original.center_high <= original.high);
    auto hint = map.erase(it);
    map.insert(hint, {original, key});
    
    dominations[team] += increment;
    sequence.capacity += increment;
}

Interval RangeBoard::undoIncrease(IvMap& map,
                              const IvMap::iterator& it,
                              const Interval& atom) {
    auto original = it->first;
    auto key = it->second;
    auto& sequence = sequences[key];
    auto team = static_cast<unsigned short>(sequence.team);
    auto begin = sequence.positions.begin();
    auto end = std::prev(sequence.positions.end());
    auto decrement = 0;

    unclassify(key, sequence, original);

    if (atom.center_low == *begin) {
        auto next = std::next(begin);
        sequence.positions.erase(begin);
        original.center_low = *next;
        int old = original.low;
        original.low = std::max(old, original.center_low - 4);
        decrement = original.low - old;
    } else {
        assert(atom.center_low == *end);
        auto prev = std::prev(end);
        sequence.positions.erase(end);
        original.center_high = *prev;
        int old = original.high;
        original.high = std::min(old, original.center_high + 4);
        decrement = old - original.high;
    }

    auto hint = map.erase(it);
    assert(original.low <= original.center_low
        && original.center_low <= original.center_high
        && original.center_high <= original.high);
    map.insert(hint, {original, key});
    
    --sequence.totalSize;
    dominations[team] -= decrement;
    classify(key, sequence, original);

    ended = ended_values.top();
    ended_values.pop();

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
    auto original = upper_it->first;
    auto lower_iv = lower_it->first;
    auto upper_key = upper_it->second;
    auto lower_key = lower_it->second;
    auto& upper_sequence = sequences[upper_key];
    auto& lower_sequence = sequences[lower_key];

    unclassify(upper_key, upper_sequence, original);
    unclassify(lower_key, lower_sequence, lower_iv);

    map.erase(upper_it);
    map.erase(lower_it);

    original.low = lower_iv.low;
    original.center_low = lower_iv.center_low;

    std::set_union(
        upper_sequence.positions.begin(),
        upper_sequence.positions.end(),
        lower_sequence.positions.begin(),
        lower_sequence.positions.end(),
        std::inserter(upper_sequence.positions,
            upper_sequence.positions.begin())
    );

    upper_sequence.totalSize += lower_sequence.totalSize;
    upper_sequence.capacity = original.size();
    upper_sequence.updateSequentiality();

    sequences.erase(lower_key);
    assert(original.low <= original.center_low
        && original.center_low <= original.center_high
        && original.center_high <= original.high);
    map.insert({original, upper_key});
    --currentSequence;
    
    ++dominations[static_cast<unsigned short>(upper_sequence.team)];
    classify(upper_key, upper_sequence, original);
}

bool Sequence::updateSequentiality() {
    auto prev = positions.begin();
    auto curr = std::next(prev);
    fragmentation = 0;
    while(curr != positions.end()) {
        fragmentation += *curr - *prev - 1;
        prev = curr;
        curr = std::next(prev);
    }
    return fragmentation == 0;
}

bool Sequence::checkWinCondition() const {
    if (totalSize < 5) {
        return false;
    }
    auto prev = positions.begin();
    auto curr = std::next(prev);
    auto counter = 1;
    while(curr != positions.end()) {
        counter++;
        if (*curr - *prev > 1) {
            counter = 1;
        }
        if (counter == 5) {
            return true;
        }
        prev = curr;
        curr = std::next(prev);
    }
    return false;
}

std::list<unsigned short> Sequence::findCriticalHoles() const {
    return {};
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

const std::array<RangeBoard::IndexMapper, 4> RangeBoard::mappers = {
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

const std::array<RangeBoard::IndexUnmapper, 4> RangeBoard::unmappers = {
    [](unsigned short index, unsigned short place) {
        return go::Position{index, place};
    },
    [](unsigned short index, unsigned short place) {
        return go::Position{place, index};
    },
    [](unsigned short index, unsigned short place) {
        unsigned short column = GomokuTraits::BOARD_DIMENSION + place - index;
        return go::Position{place, column};
    },
    [](unsigned short index, unsigned short place) {
        return go::Position{place, index - place};
    }
};


const std::array<RangeBoard::RangeChooser, 4> RangeBoard::choosers = {
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
