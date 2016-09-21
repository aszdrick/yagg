/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include <cmath>
#include "extra/macros.hpp"
#include "gomoku/BoardAnalyzer.hpp"

BoardAnalyzer::BoardAnalyzer() {
    constexpr static auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    std::vector<SequenceGroup*> sequenceGroups = {
        &rows, &columns, &mainDiagonals, &secondaryDiagonals
    };

    const auto reserved = 2 * boardDimension;
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
    history.push(position);
    searchSpace.play(position, *this);
    stoneContainer.push_back(go::Stone{position, team});
    go::Stone* stone = &stoneContainer.back();

    constexpr static auto boardDimension = GomokuTraits::BOARD_DIMENSION;
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
    sequenceIteration([&fn](const auto& sequence) {
        if (sequence.stones.size() == 4) {
            fn(sequence);
        }
    });
}

void BoardAnalyzer::sequenceIteration(const SequenceCallback& fn) const {
    for (auto& pair : sequences) {
        for (auto& seq : pair.second) {
            fn(seq);
        }
    }
}

bool BoardAnalyzer::occupied(const go::Position& position) const {
    unsigned row = position.row;
    unsigned column = position.column;
    return rows.count(row) && rows.at(row).count(column);
}

bool BoardAnalyzer::over() const {
    return hasWinner() || full();
}

bool BoardAnalyzer::hasWinner() const {
    return hasQuintuple;
}

bool BoardAnalyzer::full() const {
    return countEmptySquares() == 0;
}

unsigned BoardAnalyzer::countEmptySquares() const {
    static constexpr auto maxStones = std::pow(GomokuTraits::BOARD_DIMENSION, 2);
    return maxStones - stoneContainer.size();
}

void BoardAnalyzer::undo() {
    if (history.empty()) {
        return;
    }

    auto& position = history.top();
    constexpr static auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    unsigned row = position.row;
    unsigned column = position.column;
    rows[row].erase(column);
    columns[column].erase(row);
    mainDiagonals[boardDimension + row - column].erase(row);
    secondaryDiagonals[row + column].erase(row);
    auto it = std::find_if(stoneContainer.begin(),
        stoneContainer.end(), [&position](const auto& stone) {
            return stone.position == position;
        });
    stoneContainer.erase(it);
    searchSpace.undo(position, *this);

    bool invalidate = over();
    hasQuintuple = false;
    recalculate(position);
    if (invalidate && !over()) {
        for (auto& pair : sequences) {
            for (auto& seq : pair.second) {
                if (seq.stones.size() >= 5) {
                    hasQuintuple = true;
                    break;
                }
            }
        }
    }

    history.pop();
}

void BoardAnalyzer::recalculate(const go::Position& position) {
    constexpr static auto boardDimension = GomokuTraits::BOARD_DIMENSION;    
    unsigned row = position.row;
    unsigned column = position.column;
    std::vector<StoneGroup*> groups = {
        &rows[row], &columns[column],
        &mainDiagonals[boardDimension + row - column],
        &secondaryDiagonals[row + column]
    };

    unsigned i = 0;
    for (StoneGroup* group : groups) {
        auto report = findSequences(*group, MatchTraits::DELTA[i]);
        sequences[group] = report.sequences;
        hasQuintuple = hasQuintuple || report.foundQuintuple;
        i++;
    }
}

BoardAnalyzer::Report
BoardAnalyzer::findSequences(const StoneGroup& group, const go::Position& delta) {
    std::vector<Sequence> result;
    bool foundQuintuple = false;

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

            seq.freeEnds.first = !group.count(sequence[i] - 1);
            seq.freeEnds.second = (j >= sequence.size()
                                   || !group.count(sequence[j])
                                   || sequence[j] - sequence[j - 1] > 1);

            size_t size = seq.stones.size();
            if (size > 1 && seq.freeEnds.first + seq.freeEnds.second > 0) {
                foundQuintuple = foundQuintuple || (size >= 5);
                seq.delta = delta;
                result.push_back(std::move(seq));
            }
        }
    }

    return {result, foundQuintuple};
}
