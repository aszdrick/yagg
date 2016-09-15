/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

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
    return emptySquares.size() > 0;
}

go::State StateGenerator::generateNext() {
    assert(hasNext());
    auto currentPlayer = state.currentPlayer();
    auto newState = state;
    newState.play(emptySquares.front(), static_cast<go::Team>(currentPlayer));
    emptySquares.pop();
    return newState;
}
