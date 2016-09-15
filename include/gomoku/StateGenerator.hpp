/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef STATE_GENERATOR_HPP
#define STATE_GENERATOR_HPP

#include <queue>
#include "gomoku/CommonTypes.hpp"
#include "gomoku/State.hpp"

class StateGenerator {
 public:
    explicit StateGenerator(const go::State&);
    bool hasNext() const;
    go::State generateNext();

 private:
    std::queue<go::Position> emptySquares;
    const go::State& state;
};

#endif /* STATE_GENERATOR_HPP */
