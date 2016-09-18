/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_SEARCH_SPACE_HPP
#define GOMOKU_SEARCH_SPACE_HPP

#include <deque>
#include <set>
#include "CommonTypes.hpp"
#include "extra/macros.hpp"

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

class SearchSpace {
 public:
    SearchSpace();
    void play(const go::Position&);
    void undo(const go::Position&);

    auto& squares() const {
        return space;
    }

 private:
    std::deque<go::Position> history;
    std::set<go::Position, PositionComparator> space;
    // std::set<go::Position, PositionComparator> fragile;

    void init();
};

#endif /* GOMOKU_SEARCH_SPACE_HPP */