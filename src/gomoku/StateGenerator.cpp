/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cassert>
#include "extra/macros.hpp"
#include "gomoku/StateGenerator.hpp"
#include "gomoku/Traits.hpp"

unsigned StateGenerator::generations = 0;

StateGenerator::StateGenerator(go::State& state) : state(state) {
    generationIDs.push_back(0);
}

bool StateGenerator::hasNext() const {
    return state.emptySquares().size() > generationIDs.back();
}

const go::State& StateGenerator::generateNext() {
    auto currentPlayer = state.currentPlayer();
    // auto newState = state;
    const auto& position = nextPosition();
    generationIDs.back()++;

    state.play(position, static_cast<go::Team>(currentPlayer));
    past.push_back(position);
    // state.emptySquares().pop();
    generations++;
    generationIDs.push_back(0);
    return state;
}

Player::Move StateGenerator::command(const go::State& target) const {
    for (auto& position : past) {
        if (target.occupied(position)) {
            Player::Move command(static_cast<go::Team>(state.currentPlayer()));
            command.setPosition(position);
            return command;
        }
    }
    assert(false);
}

void StateGenerator::undo() {
    state.undo();
    generationIDs.pop_back();
}

const go::Position& StateGenerator::nextPosition() const {
    auto& skip = generationIDs.back();
    auto iterator = state.emptySquares().begin();
    for (size_t i = 0; i < skip; i++) {
        ++iterator;
    }
    return *iterator;
}
