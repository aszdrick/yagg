/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "GomokuTraits.hpp"
#include "gomoku/MatchGraphics.hpp"

namespace gomoku {

    void MatchGraphics::doUpdate(const Agent& agent, Element& element) {
        drawBoard(element);
    }

    void MatchGraphics::drawBoard(Element& window) const {
        static std::vector<sf::Vertex> lines;
        auto boardDimension = GomokuTraits::BOARD_DIMENSION;
        if (lines.empty()) {
            auto squareSize = GomokuTraits::SQUARE_SIZE;
            auto boardStart = GomokuTraits::BORDER_WIDTH;
            auto boardEnd = boardStart + (boardDimension - 1) * squareSize;
            auto offset = [=](unsigned i) {
                return boardStart + i * squareSize;
            };
            
            for (unsigned i = 0; i < boardDimension; i++) {
                lines.emplace_back(sf::Vector2f(boardStart, offset(i)));
                lines.emplace_back(sf::Vector2f(boardEnd, offset(i)));
            }

            for (unsigned i = 0; i < boardDimension; i++) {
                lines.emplace_back(sf::Vector2f(offset(i), boardStart));
                lines.emplace_back(sf::Vector2f(offset(i), boardEnd));
            }
        }

        window.draw(&lines.front(), 4 * boardDimension, sf::Lines);
    }
}

