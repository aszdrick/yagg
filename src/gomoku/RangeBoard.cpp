/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include "gomoku/RangeBoard.hpp"
#include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

void RangeBoard::play(const go::Position& position, go::Team team) {
    std::cout << position.row << " " << position.column << std::endl;
    auto start = std::chrono::system_clock::now().time_since_epoch();
    unsigned short row = position.row;
    unsigned short column = position.column;

    stones.push({position, team});
    operations_count.push(0);
    // undo_groupings.push(0);
    for (auto i = 0; i < 4; i++) {
        auto index = mappers[i](row, column);
        if (index > -1) {
            auto iv = choosers[i](row, column);
            if (!lines[i][index].count(iv)) {
                create(lines[i][index], iv, team);
            } else {
                solve(lines[i][index], iv, team);
            }
        }
    }

    auto end = std::chrono::system_clock::now().time_since_epoch();
    auto play_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (play_delay > 0) {
        TRACE(play_delay);
    }
}

void RangeBoard::undo() {
    ECHO("undo");
    auto start = std::chrono::system_clock::now().time_since_epoch();
    if (!operations_count.empty()) {
        auto& count = operations_count.top();
        if (operations.size() >= count) {
            stones.pop();
            for (auto i = 0; i < count; i++) {
                auto operation = operations.top();
                switch(operation) {
                    case Operation::CREATE:
                        undoCreate();
                        break;
                    case Operation::RESIZE:
                        undoResize();
                        break;
                    case Operation::SPLIT:
                        undoSplit();
                        break;
                }
                operations.pop();
            }
        }
        operations_count.pop();
    }
    ECHO("end undo");
    auto end = std::chrono::system_clock::now().time_since_epoch();
    auto undo_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (undo_delay > 0) {
        TRACE(undo_delay);
    }
}
    // if (!operations_count.empty()) {
    //     auto& stone = stones.top();
    //     auto& position = stone.position;
    //     for (auto i = 0; i < 4; i++) {
    //         auto index = mappers[i](position.row, position.column);
    //         auto& map = lines[i][index];
    //         if (index > -1) {
    //             auto& count = operations_count.top();
    //             if (operations.size() >= count) {
    //                 auto iv = choosers[i](position.row, position.column);
    //                 for (auto i = 0; i < count; i++) {
    //                     auto type = operations.top();
    //                     switch (type) {
    //                         case Operation::CREATE:
    //                             undoCreate();
    //                             break;
    //                         case Operation::RESIZE:
    //                             break;
    //                         case Operation::SPLIT:
    //                             break;
    //                     }
    //                     operations.pop();
    //                 }
    //             }
    //             operations_count.pop();
    //         }
    //     }
    //     stones.pop();

    // if (!undo_groupings.empty()) {
    //     auto executions = undo_groupings.top();
    //     undo_groupings.pop();
    //     ECHO(executions);
    //     if (undos.size() >= executions) {
    //         stones.pop();
    //         for (auto i = 0; i < executions; i++) {
    //             auto& undo = undos.top();
    //             undo();
    //             undos.pop();
    //         }
    //     }
    // }

void RangeBoard::solve(IvMap& map, Interval iv, go::Team team) {
    static constexpr auto mergeDistance = 5;
    auto mergeCount = 0;
    auto merges = std::array<assoc, 2>();
    auto noMerges = true;
    auto it = map.find(iv);

    while (it != map.end()) {
        auto interval = it->first;
        auto diff = interval.center_distance(iv);
        if (sequences[it->second].team == team && diff < mergeDistance) {
            merges[mergeCount] = premerge(map, it);
            noMerges = false;
            ++mergeCount;
        } else if (!resize(map, it, iv)) {
            split(map, it, iv);
            return;
        }
        it = map.find(iv);
    }
    
    if (noMerges) {
        create(map, iv, team);
    } else if (mergeCount > 1) {
        merge(map, merges, iv, team);
    } else {
        increase(map, merges[0], iv, team);
    }
}

assoc RangeBoard::premerge(IvMap& map, const IvMap::iterator& it) {
    auto pair = std::make_pair(it->first, it->second);
    map.erase(it);
    return pair;
}
    // undos.emplace([=, &map]() {
    //     map.insert(pair);
    // });

    // ++undo_groupings.top();

void RangeBoard::merge(IvMap& map, std::array<assoc,2>& merges,
                       Interval& iv, go::Team t) {
    auto low = 0, high = 1;

    if (merges[1].first < merges[0].first) {
        low = 1;
        high = 0;
    }

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
    // undos.emplace([=, &map] {
    //     ended = old;
    //     map.erase(low_piv);
    // });

    // ++undo_groupings.top();

void RangeBoard::increase(IvMap& map, assoc& pair, Interval& iv, go::Team t) {
    auto& piv = pair.first;
    auto& sequence = sequences[pair.second];
    auto distance = piv.center_distance(iv);
    auto team_key = static_cast<unsigned short>(t);
    auto increment = 0;

    sequence.placedPositions.insert(iv.center_low);

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
    // undos.emplace([=, &map]() {
    //     ended = old;
    //     map.erase(piv);
    //     dominations[team_key] -= increment;
    // });

    // ++undo_groupings.top();

void RangeBoard::split(IvMap& map, const IvMap::iterator& it, Interval& iv) {
    auto upper_key = it->second;
    auto lower_key = currentSequence++;
    auto sequence = sequences[upper_key];
    auto original_iv = it->first;
    auto upper_iv = original_iv;
    auto begin = sequence.placedPositions.begin();
    auto upper = sequence.placedPositions.upper_bound(iv.center_low);
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
    auto lowerOpening = sequence.openings.first;
    
    sequence.placedPositions.erase(begin, upper);
    sequence.totalSize -= std::distance(begin, upper);
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

    split_contexts.push({map, original_iv, lower_iv, upper_iv});
    operations.push(Operation::SPLIT);
    ++operations_count.top();

    create(map, iv, static_cast<go::Team>(1 - team_value));
}
    // undos.emplace([=, &map]() {
    //     map.erase(niv);
    //     map.erase(original);
    //     map.insert(old_pair);
    //     sequences.erase(lower_key);
    //     sequences[old_pair.second] = old_sequence;
    // });

    // ++undo_groupings.top();

void RangeBoard::undoSplit() {
    auto context = split_contexts.top();
    auto& map = context.map.get();
    auto upper_it = map.find(context.upper);
    auto lower_it = map.find(context.lower);
    auto upper_key = upper_it->second;
    auto lower_key = lower_it->second;
    auto& upper_sequence = sequences[upper_key];
    auto& lower_sequence = sequences[lower_key];

    map.erase(upper_it);
    map.erase(lower_it);

    std::set_union(
        upper_sequence.placedPositions.begin(),
        upper_sequence.placedPositions.end(),
        lower_sequence.placedPositions.begin(),
        lower_sequence.placedPositions.end(),
        std::inserter(upper_sequence.placedPositions,
            upper_sequence.placedPositions.begin())
    );

    upper_sequence.totalSize += lower_sequence.totalSize;
    upper_sequence.capacity = context.original.size();
    upper_sequence.updateSequentiality();

    ++dominations[static_cast<unsigned short>(upper_sequence.team)];

    sequences.erase(lower_key);
    map.insert({context.original, upper_key});

    split_contexts.pop();
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

    resize_contexts.push({map, original, resized});
    operations.push(Operation::RESIZE);
    ++operations_count.top();

    return true;
}
    // undos.emplace([=, &map]() {
    //     map.erase(piv);
    //     map.insert(old_pair);
    //     dominations[enemy] += decrement;
    // });

    // ++undo_groupings.top();

void RangeBoard::undoResize() {
    auto& context = resize_contexts.top();
    auto& map = context.map.get();
    auto increment = context.original.size() - context.resized.size();
    auto it = map.find(context.resized);
    auto key = it->second;
    auto& sequence = sequences[key];
    auto team = static_cast<unsigned short>(sequence.team);
    
    auto hint = map.erase(it);
    map.insert(hint, {context.original, key});
    
    dominations[team] += increment;
    sequence.capacity += increment;

    resize_contexts.pop();
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

    create_contexts.push({map, iv});
    operations.push(Operation::CREATE);
    ++operations_count.top();
}
    // undos.emplace([=, &map] {
    //     sequences.erase(key);
    //     dominations[team_key] -= increment;
    //     map.erase(iv);
    //     --currentSequence;
    // });

    // ++undo_groupings.top();

void RangeBoard::undoCreate() {
    auto& context = create_contexts.top();
    auto& map = context.map.get();
    auto it = map.find(context.created);
    auto piv = it->first;
    auto key = it->second;
    auto seq_it = sequences.find(key);
    auto team = seq_it->second.team;

    sequences.erase(seq_it);
    map.erase(it);

    dominations[static_cast<unsigned short>(team)] -= piv.size();
    --currentSequence;

    create_contexts.pop();
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
