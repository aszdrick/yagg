/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cassert>
#include "extra/macros.hpp"
#include "gomoku/StateGenerator.hpp"
#include "gomoku/Traits.hpp"

unsigned StateGenerator::generations = 0;

StateGenerator::StateGenerator(go::State& state) : state(state) {
    generatedPositions.emplace_back();
}

const go::State& StateGenerator::generateNext(bool set) {
    auto currentPlayer = state.currentPlayer();
    const auto& position = nextPosition();
    if (set) {
        last = position;
    }
    generatedPositions.back().insert(position);

    state.play(position, static_cast<go::Team>(currentPlayer));
    generations++;
    generatedPositions.emplace_back();
    return state;
}

void StateGenerator::undo() {
    state.undo();
    generatedPositions.pop_back();
}

Player::Move StateGenerator::command() const {
    Player::Move command(static_cast<go::Team>(state.currentPlayer()));
    command.setPosition(chosen);
    return command;
}

const go::Position& StateGenerator::nextPosition() const {
    auto& space = state.possibleSquares();
    auto it = space.begin();
    auto& context = generatedPositions.back();
    while (context.count(*it)) {
        assert(it != space.end());
        if (state.occupied(*it)) {
            ECHO("------------------- WARNING -------------------");
            TRACE(*it);
        }
        // TRACE(state.occupied(*it));
        std::advance(it, 1);
    }
    return *it;
}
