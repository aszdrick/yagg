/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include <cmath>
#include "extra/macros.hpp"
#include "gomoku/BoardAnalyzer.hpp"

BoardAnalyzer::BoardAnalyzer() {
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
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

    using BoardType = std::decay_t<decltype(boardDimension)>;
    for (BoardType row = 0; row < boardDimension; row++) {
        for (BoardType col = 0; col < boardDimension; col++) {
            go::Position position{
                static_cast<int>(row),
                static_cast<int>(col)
            };
            freeSquares.insert(std::move(position));
        }
    }
}

void BoardAnalyzer::play(const go::Position& position, go::Team team) {
    auto start = std::chrono::system_clock::now().time_since_epoch();
    history.push(position);
    freeSquares.erase(position);
    stoneContainer.push_back(go::Stone{position, team});
    go::Stone* stone = &stoneContainer.back();

    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    unsigned row = position.row;
    unsigned column = position.column;
    rows[row][column] = stone;
    columns[column][row] = stone;
    mainDiagonals[boardDimension + row - column][row] = stone;
    secondaryDiagonals[row + column][row] = stone;
    recalculate(position);
    auto end = std::chrono::system_clock::now().time_since_epoch();
    auto noob_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (noob_delay > 0) {
        TRACE(noob_delay);
    }
}

void BoardAnalyzer::iterate(const StoneCallback& fn) const {
    for (auto& stone : stoneContainer) {
        fn(stone);
    }
}

void BoardAnalyzer::quadrupletIteration(const SequenceCallback& fn) const {
    // for (auto& pair : quadruplets) {
    //     auto& seq = sequences.at(const_cast<StoneGroup*>(pair.first)).at(pair.second);
    //     if (seq.stones.size() == 4) {
    //         fn(seq);
    //     }
    // }
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
    // unsigned row = position.row;
    // unsigned column = position.column;
    // return rows.count(row) && rows.at(row).count(column);
    return !freeSquares.count(position);
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

    bool invalidate = over();
    hasQuintuple = false;

    auto& position = history.top();
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    unsigned row = position.row;
    unsigned column = position.column;
    rows[row].erase(column);
    columns[column].erase(row);
    mainDiagonals[boardDimension + row - column].erase(row);
    secondaryDiagonals[row + column].erase(row);
    for (auto it = stoneContainer.begin(); it != stoneContainer.end(); ++it) {
        if (it->position == position) {
            stoneContainer.erase(it);
            break;
        }
    }
    freeSquares.insert(position);

    if (invalidate) {
        for (auto& pair : sequences) {
            for (auto& seq : pair.second) {
                if (seq.stones.size() >= 5) {
                    hasQuintuple = true;
                    break;
                }
            }
        }
    }

    recalculate(position);
    history.pop();
}

void BoardAnalyzer::recalculate(const go::Position& position) {
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;    
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
                // if (size == 4) {
                //     decltype(quadruplets)::value_type pair{&group, result.size()};
                //     quadruplets.push_back(std::move(pair));
                // }
                seq.delta = delta;
                result.push_back(std::move(seq));
            }
        }
    }

    return {result, foundQuintuple};
}
