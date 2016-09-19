/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef STATE_GENERATOR_HPP
#define STATE_GENERATOR_HPP

#include "gomoku/CommonTypes.hpp"
#include "gomoku/Player.hpp"
#include "gomoku/State.hpp"

class StateGenerator {
 public:
    explicit StateGenerator(go::State&);
    const go::State& generateNext(bool = false);
    void undo();
    Player::Move command() const;
    static void reset() { generations = 0; }
    static unsigned generationCount() { return generations; }

    bool hasNext() const {
        return state.possibleSquares().size() > generatedPositions.back().size();
    }

    void remember() {
        chosen = last;
    }

 private:
    std::list<std::set<go::Position>> generatedPositions;
    static unsigned generations;
    go::State& state;
    go::Position last;
    go::Position chosen;

    const go::Position& nextPosition() const;
};

#endif /* STATE_GENERATOR_HPP */
