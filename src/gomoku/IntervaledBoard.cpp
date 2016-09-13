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
    ECHO("--------------------------------------------------");
}

void IntervaledBoard::solve(IvMap& map, Interval iv, const go::Team& team) {
    ushort mergeDistance = 5, mergeCount = 0, loopCount = 0;
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
            ECHO("split (breaking a hole)");
            auto value = splitSequence(it, iv);
            map.split(it, iv, value.first, value.second);
            return;
        }

        it = map.find(iv);
        ++loopCount;
    }
    
    if (noMerges) {
        ECHO("double resize");
        newSequence(map, iv, team);
    } else if (mergeCount > 1) {
        ECHO("double merge");

    } else if (loopCount == 2) {
        ECHO("single resize & single merge");
        map.insert(merges[0]);
        sequences[merges[0].second].size++;
    } else {
        ECHO("single merge (filling a hole)");
        
    }
}

Split IntervaledBoard::splitSequence(const IvMap::iterator& it, Interval& iv) {



    return {0, false};
}

ushort IntervaledBoard::newSequence(IvMap& map, Interval& iv, const go::Team& team) {
    ushort id = currentSequence;
    sequences[currentSequence] = {
        team,           // Team
        iv.center_low,  // Origin
        1,              // Size (main counting of stones)
        {},             // IMap holes -> counting of stones
        {true, true}    // Openings
    };
    map[iv] = id;
    ++currentSequence;
    return id;
}