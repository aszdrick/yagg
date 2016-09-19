/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_SEARCH_SPACE_HPP
#define GOMOKU_SEARCH_SPACE_HPP

#include <deque>
#include <functional>
#include <set>
#include <stack>
#include "CommonTypes.hpp"
#include "extra/macros.hpp"

class SearchSpace {
 public:
    SearchSpace() {
        init();
    }

    template<typename T>
    void play(const go::Position&, const T&);

    template<typename T>
    void undo(const go::Position&, const T&);

    auto& squares() const {
        return space;
    }

 private:
    // std::deque<go::Position> history;
    std::stack<bool> magicEntity;
    std::set<go::Position> space;
    // std::set<go::Position> fragile;

    void init();
};

#include "gomoku/SearchSpace.ipp"

#endif /* GOMOKU_SEARCH_SPACE_HPP */