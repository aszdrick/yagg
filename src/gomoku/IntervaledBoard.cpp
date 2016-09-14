/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/IntervaledBoard.hpp"
#include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

IntervaledBoard::IntervaledBoard() : currentSequence(0) { }

void IntervaledBoard::play(const go::Position& position, const go::Team& team) {
    short row = position.row;
    short column = position.column;
    short limit = GomokuTraits::BOARD_DIMENSION;
    Interval rr = {
        static_cast<short>(std::max(row - 4, 0)),
        row, row,
        static_cast<short>(std::min(row + 4, limit - 1))
    };
    Interval cc = {
        static_cast<short>(std::max(column - 4, 0)),
        column, column,
        static_cast<short>(std::min(column + 4, limit - 1))
    };

    stones.push_back({position, team});

    if (!rows[row].count(cc)) {
        // Unclaimed region
        newSequence(rows[row], cc, team);
    } else {
        // Conflicting region
        solve(rows[row], cc, team);
    }
    ECHO("---------------------- ROWS ----------------------");
    for (auto pair : rows[row]) {
        std::cout << "interval: " << pair.first << std::endl;
        std::cout << "key: " << pair.second << std::endl;
    }
    
    if (!columns[column].count(rr)) {
        // Unclaimed region
        newSequence(columns[column], rr, team);
    } else {
        // Conflicting region
        solve(columns[column], rr, team);
    }
    ECHO("-------------------- COLUMNS ---------------------");
    for (auto pair : columns[column]) {
        std::cout << "interval: " << pair.first << std::endl;
        std::cout << "key: " << pair.second << std::endl;
    }

    if (!mainDiagonals[limit + row - column].count(rr)) {
        // Unclaimed region
        newSequence(mainDiagonals[limit + row - column], rr, team);
    } else {
        // Conflicting region
        solve(mainDiagonals[limit + row - column], rr, team);
    }
    ECHO("----------------- MAIN DIAGONALS -----------------");
    for (auto pair : mainDiagonals[limit + row - column]) {
        std::cout << "interval: " << pair.first << std::endl;
        std::cout << "key: " << pair.second << std::endl;
    }

    if (!secondaryDiagonals[row + column].count(rr)) {
        // Unclaimed region
        newSequence(secondaryDiagonals[row + column], rr, team);
    } else {
        // Conflicting region
        solve(secondaryDiagonals[row + column], rr, team);
    }
    ECHO("----------------- SECD DIAGONALS -----------------");
    for (auto pair : secondaryDiagonals[row + column]) {
        std::cout << "interval: " << pair.first << std::endl;
        std::cout << "key: " << pair.second << std::endl;
    }
}

void IntervaledBoard::solve(IvMap& map, Interval iv, const go::Team& team) {
    ECHO("--------------------------------------------------");
    ushort mergeDistance = 5, mergeCount = 0;
    std::array<IvMap::assoc, 2> merges;
    bool noMerges = true;
    auto it = map.find(iv);

    while (it != map.end()) {
        auto interval = it->first;
        auto diff = interval.center_distance(iv);
        if (sequences[it->second].team == team && diff < mergeDistance) {
            merges[mergeCount] = map.premerge(it, iv);
            noMerges = false;
            ++mergeCount;
        } else if (!map.resize(it, iv)) {
            ECHO("split (breaking a sequence)");
            auto value = splitSequence(it, iv);
            map.split(it, iv, value.first, value.second);
            newSequence(map, iv, team);
            return;
        }
        it = map.find(iv);
    }
    
    if (noMerges) {
        ECHO("double resize");
        newSequence(map, iv, team);
    } else if (mergeCount > 1) {
        ECHO("double merge");
        // TODO: deal with double merge
    } else {
        ECHO("single merge");
        mergeSequence(map, merges[0], iv);
    }
}

void IntervaledBoard::mergeSequence(IvMap& map, IvMap::assoc& pair, Interval& iv) {
    auto& piv = pair.first;
    auto& sequence = sequences[pair.second];
    auto distance = piv.center_distance(iv);
    ushort adition;

    if (distance > 0) {
        // TODO: deal with hole

    } else if (distance < 0) {
        // TODO: deal with play in a hole

    } else {
        sequence.localSize++;
    }

    if (piv.center_low > iv.center_high) {
        adition = piv.low - iv.low;
        piv.low = iv.low;
        piv.center_low = iv.center_low;
    } else {
        adition = iv.high - piv.high;
        piv.high = iv.high;
        piv.center_high = iv.center_high;
    }

    sequence.capacity += adition;

    map.insert(std::move(pair));
}

Split IntervaledBoard::splitSequence(const IvMap::iterator& it, Interval& iv) {
    ushort localSize = 0, totalSize = 0, capacity = 0;
    IvMap remainingHoles;
    short origin = 0;
    bool lowSplit = true;
    
    auto id = currentSequence++;
    auto& sequence = sequences[it->second];
    auto& piv = it->first;
    auto atomic = iv;

    // Make interval atomic
    atomic.low = atomic.center_low;
    atomic.high = atomic.center_high;

    // For that shit work, holes need to have center_high = high
    // With this condition satisfied, it's guaranteed that only one hole will match
    auto hole = sequence.holes.find(atomic);
    auto hole_iv = hole->first;
    
    // iv assumes extremeties of hole
    iv.low = hole_iv.low;
    iv.high = hole_iv.high;

    localSize = hole->second;
    totalSize += localSize;
    hole = sequence.holes.erase(hole);

    if (hole_iv.center_high > sequence.origin) {
        lowSplit = false;
        origin = hole_iv.high + 1;
        capacity = piv.high - iv.center_high;

        while (hole != sequence.holes.end()) {
            totalSize += hole->second;
            remainingHoles[hole->first] = hole->second;
            hole = sequence.holes.erase(hole);
        }

    } else {
        origin = hole_iv.low - 1;
        capacity = iv.center_low - piv.low;

        while (hole != sequence.holes.begin()) {
            totalSize += hole->second;
            remainingHoles[hole->first] = hole->second;
            hole = std::prev(sequence.holes.erase(hole));
        }
    }

    sequence.totalSize -= totalSize;
    sequence.capacity -= (capacity + 1);

    sequences[id] = {
        sequence.team,
        origin,
        localSize,
        totalSize,
        capacity,
        remainingHoles
    };

    return {id, lowSplit};
}

ushort IntervaledBoard::newSequence(IvMap& map, Interval& iv, const go::Team& team) {
    ushort id = currentSequence;
    sequences[currentSequence] = {
        team,           // Team
        iv.center_low,  // Origin
        1,              // Size (main counting of stones)
        1,              // Total size
        iv.size(),      // Sequence capacity
        {}              // IMap holes -> counting of stones
    };
    map[iv] = id;
    ++currentSequence;
    return id;
}