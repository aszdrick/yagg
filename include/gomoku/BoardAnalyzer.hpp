/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_BOARD_ANALYZER_HPP
#define GOMOKU_BOARD_ANALYZER_HPP

#include "CommonTypes.hpp"
#include <functional>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class BoardAnalyzer {
 private:
    struct Sequence;
    using StoneGroup = std::unordered_map<unsigned, go::Stone*>;
    using StoneCallback = std::function<void(const go::Stone&)>;
    using SequenceReference = std::vector<std::pair<const StoneGroup*, unsigned>>;
 public:
    using SequenceCallback = std::function<void(const Sequence&)>;

    BoardAnalyzer();
    void play(const go::Position&, go::Team);
    void iterate(const StoneCallback&) const;
    void quadrupletIteration(const SequenceCallback&) const;
    void sequenceIteration(const SequenceCallback&) const;
    bool occupied(const go::Position&) const;
    bool over() const;
    bool hasWinner() const;
    bool full() const;
    unsigned countEmptySquares() const;
    void undo();

    auto& emptySquares() {
        return freeSquares;
    }

 private:
    struct Report;
    using SequenceGroup = std::unordered_map<unsigned, StoneGroup>;

    std::vector<go::Stone> stoneContainer;
    SequenceGroup rows;
    SequenceGroup columns;
    SequenceGroup mainDiagonals;
    SequenceGroup secondaryDiagonals;
    std::unordered_map<StoneGroup*, std::vector<Sequence>> sequences;
    bool hasQuintuple = false;
    std::stack<go::Position> history;
    std::unordered_set<go::Position> freeSquares;

    void recalculate(const go::Position&);
    Report findSequences(const StoneGroup&, const go::Position&);
};

struct BoardAnalyzer::Sequence {
    std::vector<go::Stone*> stones;
    std::pair<bool, bool> freeEnds = {false, false};
    go::Position delta;
};

struct BoardAnalyzer::Report {
    std::vector<Sequence> sequences;
    bool foundQuintuple = false;
};

#endif /* GOMOKU_BOARD_ANALYZER_HPP */
