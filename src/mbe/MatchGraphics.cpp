/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "base/Game.hpp"
#include "macros.hpp"
#include "GomokuTraits.hpp"
#include <SFML/Graphics.hpp>

namespace {
    void drawBoard(sf::RenderWindow& window) {
        static std::vector<sf::Vertex> lines;
        auto boardDimension = GomokuTraits::BOARD_DIMENSION;
        if (lines.empty()) {
            auto squareSize = GomokuTraits::SQUARE_SIZE;
            auto boardStart = GomokuTraits::BORDER_WIDTH;
            auto boardEnd = boardStart + (boardDimension - 1) * squareSize;
            auto offset = [=](unsigned i) { return boardStart + i * squareSize; };
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

template<>
void GameState<sf::RenderWindow>::updateRenderer(sf::RenderWindow& window) {
    // Draw shit here
    drawBoard(window);
}

