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

const go::State& StateGenerator::generateNext(bool set) {
    auto currentPlayer = state.currentPlayer();
    const auto& position = nextPosition();
    if (set) {
        last = position;
    }
    generationIDs.back()++;

    state.play(position, static_cast<go::Team>(currentPlayer));
    generations++;
    generationIDs.push_back(0);
    return state;
}

void StateGenerator::undo() {
    state.undo();
    generationIDs.pop_back();
}

void StateGenerator::remember() {
    chosen = last;
}

Player::Move StateGenerator::command() const {
    Player::Move command(static_cast<go::Team>(state.currentPlayer()));
    command.setPosition(chosen);
    return command;
}

const go::Position& StateGenerator::nextPosition() const {
    auto& skip = generationIDs.back();
    // for (size_t i = 0; i < skip; i++) {
    //     ++iterator;
    // }
    return *std::next(state.emptySquares().begin(), skip);
}
