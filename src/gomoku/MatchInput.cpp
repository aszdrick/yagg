/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/MatchInput.hpp"
#include "GomokuTraits.hpp"
#include "macros.hpp"

namespace gomoku {
    void MatchInput::doUpdate(const Agent& agent, Element& list) {
        while (!list.empty()) {
            auto event = list.front();
            list.pop_front();

            switch(event.type) {
                case sf::Event::MouseButtonPressed:
                    ECHO(handleMousePressed(event.mouseButton.x,
                        event.mouseButton.y));
                    break;
                default: break;
            }
        }
    }

    std::pair<int, int> MatchInput::handleMousePressed(float x, float y) {
        auto boardStart = GomokuTraits::BORDER_WIDTH;
        auto squareSize = GomokuTraits::SQUARE_SIZE;
        unsigned column = static_cast<unsigned>((x - boardStart) / squareSize);
        unsigned row = static_cast<unsigned>((y - boardStart) / squareSize);
        // assert(column < GomokuTraits::BOARD_DIMENSION);
        // assert(row < GomokuTraits::BOARD_DIMENSION);
        return {row, column};
    }
}

