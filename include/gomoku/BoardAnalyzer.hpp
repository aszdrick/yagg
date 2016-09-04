/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_BOARD_ANALYZER_HPP
#define GOMOKU_BOARD_ANALYZER_HPP

#include "CommonTypes.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

class BoardAnalyzer {
 public:
    using StoneGroup = std::unordered_map<unsigned, go::Stone*>;
    using Callback = std::function<void(const go::Stone&)>;

    BoardAnalyzer();
    void play(const go::Position&, go::Team);
    void iterate(const Callback&) const;
    bool isOccupied(const go::Position&) const;
    bool isOver() const;

 private:
    struct Sequence;
    struct Report;
    using SequenceGroup = std::unordered_map<unsigned, StoneGroup>;

    std::vector<go::Stone> stoneContainer;
    SequenceGroup rows;
    SequenceGroup columns;
    SequenceGroup mainDiagonals;
    SequenceGroup secondaryDiagonals;
    std::unordered_map<StoneGroup*, std::vector<Sequence>> sequences;
    bool hasQuintuple = false;

    void recalculate(const go::Position&);
    Report findSequences(const StoneGroup&) const;
    unsigned distance(const go::Position&, const go::Position&) const;
};

struct BoardAnalyzer::Sequence {
    std::vector<go::Stone*> stones;
    unsigned freeEnds = 0;
};

struct BoardAnalyzer::Report {
    std::vector<Sequence> sequences;
    bool foundQuintuple = false;
};

#endif /* GOMOKU_BOARD_ANALYZER_HPP */
