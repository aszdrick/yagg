/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "GomokuTraits.hpp"
#include "gomoku/MatchGraphics.hpp"

namespace gomoku {

    void MatchGraphics::doUpdate(Agent& match, Element& window) {
        drawBoard(window);
        drawBalls(match, window);
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

        window.clear(sf::Color::Black);
        window.draw(&lines.front(), 4 * boardDimension, sf::Lines);
    }
    
    void MatchGraphics::drawBalls(Agent& match, Element& window) const {
        auto& stones = match.getState().getStones();
        for (auto& stone : stones) {
            auto shape = sf::CircleShape(GomokuTraits::STONE_RADIUS);
            auto squareSize = GomokuTraits::SQUARE_SIZE;
            shape.setPosition(sf::Vector2f(
                squareSize + stone.column * squareSize - GomokuTraits::STONE_RADIUS,
                squareSize + stone.row * squareSize - GomokuTraits::STONE_RADIUS));

            auto white = GomokuTraits::WHITE_COLOR;
            auto black = GomokuTraits::BLACK_COLOR;
            shape.setFillColor(stone.team == Team::WHITE ? white : black);
            window.draw(shape);
        }
    }
}

