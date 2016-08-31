/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/MatchInput.hpp"

namespace gomoku {
    void MatchInput::doUpdate(const Agent& agent, Element& element) {

    }
}

// int GomokuInterface::handleMousePressed(double x, double y) {
//     auto boardStart = GomokuTraits::BORDER_WIDTH;
//     auto squareSize = GomokuTraits::SQUARE_SIZE;
//     unsigned column = static_cast<unsigned>((x - boardStart) / squareSize);
//     unsigned row = static_cast<unsigned>((y - boardStart) / squareSize);
//     assert(column < GomokuTraits::BOARD_DIMENSION);
//     assert(row < GomokuTraits::BOARD_DIMENSION);
//     stones.push_back({Team::BLACK, row, column});
//     return 0;
// }
