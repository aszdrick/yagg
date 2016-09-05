/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_BOARD_ANALYZER_HPP
#define GOMOKU_BOARD_ANALYZER_HPP

#include "CommonTypes.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

class BoardAnalyzer {
 private:
    struct Sequence;
    using StoneGroup = std::unordered_map<unsigned, go::Stone*>;
    using StoneCallback = std::function<void(const go::Stone&)>;
    using SequenceCallback = std::function<void(const Sequence&)>;
    using SequenceReference = std::vector<std::pair<const StoneGroup*, unsigned>>;
 public:

    BoardAnalyzer();
    void play(const go::Position&, go::Team);
    void iterate(const StoneCallback&) const;
    void quadrupletIteration(const SequenceCallback&) const;
    bool isOccupied(const go::Position&) const;
    bool isOver() const;

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
    SequenceReference quadruplets;

    void recalculate(const go::Position&);
    Report findSequences(const StoneGroup&);
};

struct BoardAnalyzer::Sequence {
    std::vector<go::Stone*> stones;
    std::pair<bool, bool> freeEnds = {false, false};
};

struct BoardAnalyzer::Report {
    std::vector<Sequence> sequences;
    bool foundQuintuple = false;
};

#endif /* GOMOKU_BOARD_ANALYZER_HPP */
