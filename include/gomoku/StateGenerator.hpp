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
    explicit StateGenerator(go::State&);
    bool hasNext() const;
    const go::State& generateNext(bool = false);
    Player::Move command(const go::State&) const;
    void undo();
    static void reset() { generations = 0; }
    static unsigned generationCount() { return generations; }

 private:
    std::vector<unsigned> generationIDs;
    static unsigned generations;
    go::State& state;
    go::Position first;

    const go::Position& nextPosition() const;
};

#endif /* STATE_GENERATOR_HPP */
