/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cmath>
#include "extra/macros.hpp"
#include "gomoku/BoardAnalyzer.hpp"
#include "gomoku/Traits.hpp"

BoardAnalyzer::BoardAnalyzer() {
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

void BoardAnalyzer::play(const go::Position& position, go::Team team) {
    // BLANK
    stoneContainer.push_back(go::Stone{position, team});
    go::Stone* stone = &stoneContainer.back();
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

void BoardAnalyzer::iterate(const Callback& fn) const {
    for (auto& stone : stoneContainer) {
        fn(stone);
    }
}

bool BoardAnalyzer::isOccupied(const go::Position& position) const {
    unsigned row = position.row;
    unsigned column = position.column;
    return rows.count(row) && rows.at(row).count(column);
}

bool BoardAnalyzer::isOver() const {
    return hasQuintuple;
}

void BoardAnalyzer::recalculate(const go::Position& position) {
    unsigned boardDimension = GomokuTraits::BOARD_DIMENSION;    
    unsigned row = position.row;
    unsigned column = position.column;
    std::vector<StoneGroup*> groups = {
        &rows[row], &columns[column],
        &mainDiagonals[boardDimension + row - column],
        &secondaryDiagonals[row + column]
    };

    std::vector<std::string> labels = {
        "Row: ", "Column: ", "Main diagonal: ",
        "Secondary diagonal: "
    };

    ECHO("----------------------");
    BLANK
    int i = 0;
    for (StoneGroup* group : groups) {
        ECHO(labels[i++]);
        auto report = findSequences(*group);
        sequences[group] = report.sequences;
        hasQuintuple = hasQuintuple || report.foundQuintuple;
    }
}

BoardAnalyzer::Report
BoardAnalyzer::findSequences(const StoneGroup& group) const {
    // ECHO("----------------------------");
    std::vector<Sequence> result;
    bool foundQuintuple = false;

    Sequence currentSequence;
    go::Stone* lastPtr;
    bool isFirst = true;
    bool empty = false;
    bool onHold = false;

    auto setFirst = [&](go::Stone* const ptr) {
        currentSequence.stones.push_back(ptr);
        if (empty) {
            currentSequence.freeEnds.first = true;
        }
        lastPtr = ptr;
        isFirst = false;
        onHold = false;
    };

    auto pushSequence = [&]() {
        // pushes the current sequence to the result if it's big enough
        size_t size = currentSequence.stones.size();
        if (size > 1) {
            foundQuintuple = foundQuintuple || (size >= 5);
            result.push_back(std::move(currentSequence));
        }
        onHold = (size > 1);
        currentSequence.stones.clear();
        currentSequence.freeEnds = {false, false};
        isFirst = true;
    };

    for (unsigned i = 0; i < GomokuTraits::BOARD_DIMENSION; i++) {
        if (!group.count(i)) {
            empty = true;
            if (onHold) {
                result.back().freeEnds.second = true;
            }
            continue;
        }
        auto& stonePtr = group.at(i);
        // TRACE(currentSequence.stones.size());
        if (isFirst) {
            setFirst(stonePtr);
            empty = false;
            continue;
        }

        go::Stone& stone = *stonePtr;
        go::Stone& last = *lastPtr;
        // TRACE(static_cast<int>(stone.team));
        // TRACE(static_cast<int>(last.team));
        unsigned dist = distance(stone.position, last.position);
        if (dist > 1 || stone.team != last.team) {
            // unable to continue the sequence
            pushSequence();
            setFirst(stonePtr);
            empty = false;
            continue;
        }

        currentSequence.stones.push_back(stonePtr);
        lastPtr = stonePtr;
        empty = false;
    }

    pushSequence();
    if (empty && onHold) {
        result.back().freeEnds.second = true;
    }

    // TRACE(result.size());
    for (auto& sequence : result) {
        // TRACE(sequence.stones.size());
        for (auto& stone : sequence.stones) {
            std::cout << *stone << std::endl;
        }
        TRACE(sequence.freeEnds);
        BLANK
    }
    return {result, foundQuintuple};
}

unsigned BoardAnalyzer::distance(const go::Position& first,
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
