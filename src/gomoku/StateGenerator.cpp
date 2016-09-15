/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cassert>
#include "extra/macros.hpp"
#include "gomoku/StateGenerator.hpp"
#include "gomoku/Traits.hpp"

StateGenerator::StateGenerator(const go::State& state) : state(state) {
    constexpr auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    using BoardType = std::decay_t<decltype(boardDimension)>;
    for (BoardType row = 0; row < boardDimension; row++) {
        for (BoardType col = 0; col < boardDimension; col++) {
            go::Position position{
                static_cast<int>(row),
                static_cast<int>(col)
            };
            if (!state.occupied(position)) {
                emptySquares.push(std::move(position));
            }
        }
    }
}

bool StateGenerator::hasNext() const {
    return !emptySquares.empty();
}

go::State StateGenerator::generateNext() {
    auto currentPlayer = state.currentPlayer();
    auto newState = state;
    auto& position = emptySquares.front();
    newState.play(position, static_cast<go::Team>(currentPlayer));
    past.push_back(position);
    emptySquares.pop();
    return newState;
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
