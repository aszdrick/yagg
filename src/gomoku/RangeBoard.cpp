/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include "gomoku/RangeBoard.hpp"
#include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

RangeBoard::RangeBoard() : currentSequence(0) { }

void RangeBoard::play(const go::Position& position, const go::Team& team) {
    auto start = std::chrono::system_clock::now().time_since_epoch();
    unsigned short row = position.row;
    unsigned short column = position.column;
    unsigned short limit = GomokuTraits::BOARD_DIMENSION;
    Interval rr = {
        static_cast<unsigned short>(std::max(row - 4, 0)),
        row, row,
        static_cast<unsigned short>(std::min(row + 4, limit - 1))
    };
    Interval cc = {
        static_cast<unsigned short>(std::max(column - 4, 0)),
        column, column,
        static_cast<unsigned short>(std::min(column + 4, limit - 1))
    };

    stones.push_back({position, team});

    if (!rows[row].count(cc)) {
        // Unclaimed region
        newSequence(rows[row], cc, team);
    } else {
        // Conflicting region
        solve(rows[row], cc, team);
    }
    // ECHO("---------------------- ROWS ----------------------");
    // for (auto pair : rows[row]) {
    //     std::cout << "interval: " << pair.first << std::endl;
    //     std::cout << "key: " << pair.second << std::endl;
    // }
    
    if (!columns[column].count(rr)) {
        // Unclaimed region
        newSequence(columns[column], rr, team);
    } else {
        // Conflicting region
        solve(columns[column], rr, team);
    }
    // ECHO("-------------------- COLUMNS ---------------------");
    // for (auto pair : columns[column]) {
    //     std::cout << "interval: " << pair.first << std::endl;
    //     std::cout << "key: " << pair.second << std::endl;
    // }

    if (!mainDiagonals[limit + row - column].count(rr)) {
        // Unclaimed region
        newSequence(mainDiagonals[limit + row - column], rr, team);
    } else {
        // Conflicting region
        solve(mainDiagonals[limit + row - column], rr, team);
    }
    // ECHO("----------------- MAIN DIAGONALS -----------------");
    // for (auto pair : mainDiagonals[limit + row - column]) {
    //     std::cout << "interval: " << pair.first << std::endl;
    //     std::cout << "key: " << pair.second << std::endl;
    // }

    if (!secondaryDiagonals[row + column].count(rr)) {
        // Unclaimed region
        newSequence(secondaryDiagonals[row + column], rr, team);
    } else {
        // Conflicting region
        solve(secondaryDiagonals[row + column], rr, team);
    }
    // ECHO("----------------- SECD DIAGONALS -----------------");
    // for (auto pair : secondaryDiagonals[row + column]) {
    //     std::cout << "interval: " << pair.first << std::endl;
    //     std::cout << "key: " << pair.second << std::endl;
    // }
    auto end = std::chrono::system_clock::now().time_since_epoch();
    auto play_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (play_delay > 0) {
        TRACE(play_delay);
    }
}

void RangeBoard::solve(IvMap& map, Interval iv, const go::Team& team) {
    // ECHO("--------------------------------------------------");
    auto mergeDistance = 5, mergeCount = 0;
    auto merges = std::array<assoc, 2>();
    auto noMerges = true;
    auto it = map.find(iv);

    while (it != map.end()) {
        auto interval = it->first;
        auto diff = interval.center_distance(iv);
        if (sequences[it->second].team == team && diff < mergeDistance) {
            auto piv = it->first;
            auto data = it->second;
            map.erase(it);
            merges[mergeCount] = {piv, data};
            noMerges = false;
            ++mergeCount;
        } else if (!resize(map, it, iv)) {
            auto pair = splitSequence(it, iv);
            auto hint = map.erase(it);
            hint = map.insert(hint, pair.first);
            map.insert(hint, pair.second);
            newSequence(map, iv, team);
            return;
        }
        it = map.find(iv);
    }
    
    if (noMerges) {
        newSequence(map, iv, team);
    } else if (mergeCount > 1) {
        mergeSequence(map, merges, iv);
    } else {
        mergeSequence(map, merges[0], iv);
    }
}

void RangeBoard::mergeSequence(IvMap& map,
                            std::array<assoc, 2>& merges,
                            Interval& iv) {
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

    ended = ended || (low_sequence.sequential && low_sequence.totalSize > 4);
}

void RangeBoard::mergeSequence(IvMap& map, assoc& pair, Interval& iv) {
    auto& piv = pair.first;
    auto& sequence = sequences[pair.second];
    auto distance = piv.center_distance(iv);

    sequence.placedPositions.insert(iv.center_low);

    if (distance < 0) {
        sequence.updateSequentiality();
    } else {
        sequence.sequential = sequence.sequential && distance == 0;
    }

    if (distance >= 0) {
        if (piv.center_low > iv.center_high) {
            piv.low = iv.low;
            piv.center_low = iv.center_low;
            sequence.openings.first = (iv.center_low - iv.low) > 0;
        } else {
            piv.high = iv.high;
            piv.center_high = iv.center_high;
            sequence.openings.second = (iv.high - iv.center_high) > 0;
        }
    }
    
    ++sequence.totalSize;
    sequence.capacity = piv.size();
    map.insert(std::move(pair));

    ended = ended || (sequence.sequential && sequence.totalSize > 4);
}

Split RangeBoard::splitSequence(const IvMap::iterator& it, Interval& iv) {
    auto piv_key = it->second;
    auto niv_key = currentSequence++;
    auto sequence = sequences[piv_key];
    auto piv = it->first;

    auto begin = sequence.placedPositions.begin();
    auto upper = sequence.placedPositions.upper_bound(iv.center_low);
    auto lower = std::prev(upper);

    if (upper == sequence.placedPositions.end()) {
        std::cout << "OLD PEOPLE BURNING!!!" << std::endl;
        throw 666;
    }

    if (lower == sequence.placedPositions.end()) {
        std::cout << "PUT YOUR HANDS UP!!!" << std::endl;
        throw 666;
    }

    iv.low = *lower + 1;
    iv.high = *upper - 1;

    auto low = piv.low;
    piv.low = iv.center_high + 1;
    piv.center_low = *upper;

    auto niv = Interval{
        low,
        *begin,
        *lower,
        static_cast<unsigned short>(iv.center_low - 1)
    };
    auto capacity = niv.size();
    auto lowerPositions = std::set<unsigned short>(begin, upper);
    auto lowerOpening = sequence.openings.first;
    
    sequence.placedPositions.erase(begin, upper);
    sequence.totalSize -= std::distance(begin, upper);
    sequence.capacity -= capacity;
    sequence.openings.first = (piv.center_low - iv.center_high) > 1;
    sequence.updateSequentiality();

    sequences[niv_key] = {
        sequence.team,
        static_cast<unsigned short>(lowerPositions.size()),
        capacity,
        true,
        {lowerOpening, (iv.center_low - niv.center_high) > 1},
        lowerPositions
    };

    sequences[niv_key].updateSequentiality();

    return {{niv, niv_key}, {piv, piv_key}};
}

bool RangeBoard::resize(IvMap& map, const IvMap::iterator& it, Interval& iv) {
    auto piv = it->first;
    auto& data = it->second;

    if (iv.center_low > piv.center_low && iv.center_high < piv.center_high) {
        return false;
    }

    if (piv.center_low > iv.center_high) {
        piv.low = iv.center_high + 1;
        iv.high = piv.center_low - 1;
    } else {
        piv.high = iv.center_low - 1;
        iv.low = piv.center_high + 1;
    }

    auto hint = map.erase(it);
    map.insert(hint, {piv, data});
    return true;
}

unsigned short RangeBoard::newSequence(IvMap& map,
                                    Interval& iv,
                                    const go::Team& team) {
    unsigned short id = currentSequence;
    sequences[currentSequence] = {
        team,           // Team
        1,              // Total size
        iv.size(),      // Sequence capacity
        true,           // Sequential
        {true, true},   // Openings
        {iv.center_low} // Placed positions
    };
    map[iv] = id;
    ++currentSequence;
    return id;
}

bool RangeBoard::occupied(const go::Position& position) const {
    unsigned short row = position.row;
    unsigned short column = position.column;
    Interval cc = {column, column, column, column};
    return rows.count(row) && rows.at(row).count(cc);
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
    return fragmented;
}
