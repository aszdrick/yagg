/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_BOARD_ANALYZER_HPP
#define GOMOKU_BOARD_ANALYZER_HPP

#include "CommonTypes.hpp"
#include "gomoku/Traits.hpp"
#include <functional>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct PositionComparator {
    bool operator()(const go::Position& lhs, const go::Position& rhs) const {
        constexpr static auto ref = go::Position{7, 7};
        auto d1 = go::Position::distance(ref, lhs) + lhs.row/100.0 + lhs.column/1000.0;
        auto d2 = go::Position::distance(ref, rhs) + rhs.row/100.0 + rhs.column/1000.0;
        return d1 < d2;
        // constexpr static auto size = GomokuTraits::BOARD_DIMENSION;
        // auto first = lhs.row * size + lhs.column;
        // auto second = rhs.row * size + rhs.column;
        // return first < second;
    }
};

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
    std::set<go::Position, PositionComparator> freeSquares;

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
