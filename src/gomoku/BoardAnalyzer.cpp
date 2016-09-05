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

void BoardAnalyzer::iterate(const StoneCallback& fn) const {
    for (auto& stone : stoneContainer) {
        fn(stone);
    }
}

void BoardAnalyzer::quadrupletIteration(const SequenceCallback& fn) const {
    for (auto& pair : quadruplets) {
        fn(sequences.at(const_cast<StoneGroup*>(pair.first)).at(pair.second));
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

    // std::vector<std::string> labels = {
    //     "Row: ", "Column: ", "Main diagonal: ",
    //     "Secondary diagonal: "
    // };

    // ECHO("----------------------");
    // BLANK
    // int i = 0;
    for (StoneGroup* group : groups) {
        // ECHO(labels[i++]);
        auto report = findSequences(*group);
        sequences[group] = report.sequences;
        hasQuintuple = hasQuintuple || report.foundQuintuple;
    }
}

BoardAnalyzer::Report
BoardAnalyzer::findSequences(const StoneGroup& group) {
    // ECHO("----------------------------");
    std::vector<Sequence> result;
    bool foundQuintuple = false;

    // Sequence currentSequence;
    std::vector<std::vector<unsigned>> partialSequences;

    for (unsigned i = 0; i < GomokuTraits::BOARD_DIMENSION; i++) {
        if (!group.count(i)) {
            continue;
        }
        auto& stonePtr = group.at(i);
        if (partialSequences.size() == 0
            || group.at(partialSequences.back().back())->team != stonePtr->team) {

            partialSequences.push_back(std::vector<unsigned>());
        }
        partialSequences.back().push_back(i);
    }

    for (auto& sequence : partialSequences) {
        for (unsigned i = 0; i < sequence.size(); i++) {
            Sequence seq;
            unsigned j = i;
            while (j < sequence.size() && sequence[j] - sequence[i] <= 4) {
                seq.stones.push_back(group.at(sequence[j]));
                j++;
            }

            if (!group.count(sequence[i] - 1)) {
                seq.freeEnds.first = true;
            }

            if (j < sequence.size() && !group.count(sequence[j])) {
                seq.freeEnds.second = true;
            }

            size_t size = seq.stones.size();
            foundQuintuple = foundQuintuple || (size >= 5);
            if (size == 4) {
                decltype(quadruplets)::value_type pair{&group, result.size()};
                quadruplets.push_back(std::move(pair));
            }
            result.push_back(std::move(seq));
        }
    }

    // TRACE(result.size());
    // for (auto& sequence : result) {
    //     // TRACE(sequence.stones.size());
    //     for (auto& stone : sequence.stones) {
    //         std::cout << *stone << std::endl;
    //     }
    //     TRACE(sequence.freeEnds);
    //     BLANK
    // }
    return {result, foundQuintuple};
}
