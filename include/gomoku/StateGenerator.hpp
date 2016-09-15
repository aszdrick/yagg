/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef STATE_GENERATOR_HPP
#define STATE_GENERATOR_HPP

#include <queue>
#include "gomoku/CommonTypes.hpp"
#include "gomoku/Player.hpp"
#include "gomoku/State.hpp"

class StateGenerator {
 public:
    explicit StateGenerator(const go::State&);
    bool hasNext() const;
    go::State generateNext();
    Player::Move command(const go::State&) const;
    static void reset() { generations = 0; }
    static unsigned generationCount() { return generations; }

 private:
    std::queue<go::Position> emptySquares;
    std::vector<go::Position> past;
    static unsigned generations;
    const go::State& state;
};

#endif /* STATE_GENERATOR_HPP */
