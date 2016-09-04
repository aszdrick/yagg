/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_BOARD_ANALYZER_HPP
#define GOMOKU_BOARD_ANALYZER_HPP

#include "CommonTypes.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

namespace go {
    class BoardAnalyzer {
     public:
        using StoneGroup = std::unordered_map<unsigned, Stone*>;
        using Callback = std::function<void(const Stone&)>;

        BoardAnalyzer();
        void play(const go::Position&, go::Team);
        void iterate(const Callback&) const;
        bool isOccupied(const go::Position&) const;
        bool isOver() const;

     private:
        struct Sequence;
        struct Report;
        using SequenceGroup = std::unordered_map<unsigned, StoneGroup>;

        std::vector<Stone> stoneContainer;
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

    struct go::BoardAnalyzer::Sequence {
        std::vector<Stone*> stones;
        unsigned freeEnds = 0;
    };

    struct go::BoardAnalyzer::Report {
        std::vector<Sequence> sequences;
        bool foundQuintuple = false;
    };
}

#endif /* GOMOKU_BOARD_ANALYZER_HPP */
