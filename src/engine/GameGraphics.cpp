/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "engine/GameGraphics.hpp"
#include "core/GomokuGame.hpp"
#include "macros.hpp"

GameGraphics::GraphicalComponent() {

}

void GameGraphics::update(const GomokuGame& game, sf::RenderWindow& window) {
    // Draw shit here
    drawBoard(window);
}

void GameGraphics::drawBoard(sf::RenderWindow& window) const {
    static std::vector<sf::Vertex> lines;
    if (lines.empty()) {
        for (unsigned i = 0; i < BOARD_SIZE; i++) {
            lines.emplace_back(sf::Vector2f(
                BORDER_WIDTH, BORDER_WIDTH + i * SQUARE_SIZE));
            lines.emplace_back(sf::Vector2f(
                BORDER_WIDTH + (BOARD_SIZE - 1) * SQUARE_SIZE,
                BORDER_WIDTH + i * SQUARE_SIZE));
        }

        for (unsigned i = 0; i < BOARD_SIZE; i++) {
            lines.emplace_back(sf::Vector2f(
                BORDER_WIDTH + i * SQUARE_SIZE, BORDER_WIDTH));
            lines.emplace_back(sf::Vector2f(
                BORDER_WIDTH + i * SQUARE_SIZE,
                BORDER_WIDTH + (BOARD_SIZE - 1) * SQUARE_SIZE));
        }
    }

    window.draw(&lines.front(), 4 * BOARD_SIZE, sf::Lines);
}