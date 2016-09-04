/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <cmath>
#include "extra/macros.hpp"
#include "gomoku/BoardAnalyzer.hpp"
#include "gomoku/Traits.hpp"

go::BoardAnalyzer::BoardAnalyzer() {
    unsigned boardDimension = GomokuTraits::BOARD_DIMENSION;
    std::vector<SequenceGroup*> sequenceGroups = {
        &rows, &columns, &mainDiagonals, &secondaryDiagonals
    };

    const unsigned reserved = 2 * boardDimension;
    for (SequenceGroup* seqGroupPtr : sequenceGroups) {
        auto& seqGroup = *seqGroupPtr;
        seqGroup.reserve(reserved);
        for (unsigned i = 0; i < reserved; i++) {
            seqGroup[i].reserve(boardDimension);
        }
    }

    stoneContainer.reserve(boardDimension * boardDimension);
}

void go::BoardAnalyzer::play(const go::Position& position, go::Team team) {
    // BLANK
    stoneContainer.push_back(go::Stone{position, team});
    Stone* stone = &stoneContainer.back();
    // auto p = std::make_pair(stone->position.row, stone->position.column);
    // TRACE(p);

    unsigned boardDimension = GomokuTraits::BOARD_DIMENSION;
    unsigned row = position.row;
    unsigned column = position.column;
    rows[row][column] = stone;
    columns[column][row] = stone;
    mainDiagonals[boardDimension + row - column][row] = stone;
    secondaryDiagonals[row + column][row] = stone;
    recalculate(position);
}

void go::BoardAnalyzer::iterate(const Callback& fn) const {
    for (auto& stone : stoneContainer) {
        fn(stone);
    }
}

bool go::BoardAnalyzer::isOccupied(const go::Position& position) const {
    unsigned row = position.row;
    unsigned column = position.column;
    return rows.count(row) && rows.at(row).count(column);
}

bool go::BoardAnalyzer::isOver() const {
    return hasQuintuple;
}

void go::BoardAnalyzer::recalculate(const go::Position& position) {
    unsigned boardDimension = GomokuTraits::BOARD_DIMENSION;    
    unsigned row = position.row;
    unsigned column = position.column;
    std::vector<StoneGroup*> groups = {
        &rows[row], &columns[column],
        &mainDiagonals[boardDimension + row - column],
        &secondaryDiagonals[row + column]
    };

    // std::vector<std::string> labels = {
    //     "Row: ", "Column: ", "Main diagonal: ",
    //     "Secondary diagonal: "
    // };

    // int i = 0;
    for (StoneGroup* group : groups) {
        // ECHO(labels[i++]);
        auto report = findSequences(*group);
        sequences[group] = report.sequences;
        hasQuintuple = hasQuintuple || report.foundQuintuple;
    }
}

go::BoardAnalyzer::Report
go::BoardAnalyzer::findSequences(const StoneGroup& group) const {
    // ECHO("----------------------------");
    std::vector<Sequence> result;
    bool foundQuintuple = false;

    Sequence currentSequence;
    Stone* lastPtr;
    bool isFirst = true;

    auto setFirst = [&](Stone* const ptr) {
        currentSequence.stones.push_back(ptr);
        lastPtr = ptr;
        isFirst = false;
    };

    auto pushSequence = [&]() {
        // pushes the current sequence to the result if it's big enough
        size_t size = currentSequence.stones.size();
        if (size > 1) {
            foundQuintuple = foundQuintuple || (size >= 5);
            // ECHO("APPEND");
            result.push_back(std::move(currentSequence));
        }
        currentSequence.stones.clear();
        isFirst = true;
    };

    for (auto& pair : group) {
        auto& stonePtr = pair.second;
        // TRACE(currentSequence.stones.size());
        if (isFirst) {
            setFirst(stonePtr);
            continue;
        }

        Stone& stone = *stonePtr;
        Stone& last = *lastPtr;
        // TRACE(static_cast<int>(stone.team));
        // TRACE(static_cast<int>(last.team));
        // std::cout << stone << std::endl;
        // std::cout << last << std::endl;
        unsigned dist = distance(stone.position, last.position);
        if (dist > 3 || stone.team != last.team) {
            // unable to continue the sequence
            // TRACE(dist);
            // ECHO("BREAK");
            pushSequence();
            setFirst(stonePtr);
            continue;
        }

        currentSequence.stones.push_back(stonePtr);
        lastPtr = stonePtr;
    }

    pushSequence();

    // TRACE(result.size());
    // for (auto& sequence : result) {
    //     // TRACE(sequence.stones.size());
    //     for (auto& stone : sequence.stones) {
    //         std::cout << *stone << std::endl;
    //     }
    //     BLANK
    // }
    return {result, foundQuintuple};
}

unsigned go::BoardAnalyzer::distance(const go::Position& first,
                                     const go::Position& second) const {
    unsigned r1 = first.row;
    unsigned r2 = second.row;
    unsigned c1 = first.column;
    unsigned c2 = second.column;
    auto deltaRow = (r1 > r2) ? r1 - r2 : r2 - r1;
    auto deltaColumn = (c1 > c2) ? c1 - c2 : c2 - c1;
    if (deltaRow == deltaColumn) {
        // same diagonal
        return deltaRow;
    }

    // manhattan distance
    return deltaRow + deltaColumn;
}