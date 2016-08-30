/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <cassert>
#include "engine/GomokuInterface.hpp"

const sf::Color GomokuInterface::WHITE_COLOR(128, 128, 128);
const sf::Color GomokuInterface::BLACK_COLOR(102, 51, 0);

void GomokuInterface::drawBoard(sf::RenderWindow& window) const {
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

void GomokuInterface::drawBalls(sf::RenderWindow& window) const {
    for (auto& stone : stones) {
        auto shape = sf::CircleShape(Stone::RADIUS);
        shape.setPosition(sf::Vector2f(
            SQUARE_SIZE + stone.column * SQUARE_SIZE - Stone::RADIUS,
            SQUARE_SIZE + stone.row * SQUARE_SIZE - Stone::RADIUS));
        shape.setFillColor(stone.team == Team::WHITE ? WHITE_COLOR : BLACK_COLOR);
        window.draw(shape);
    }
}

void GomokuInterface::draw(sf::RenderWindow& window) const {
    drawBoard(window);
    drawBalls(window);
}

int GomokuInterface::handleMousePressed(double x, double y) {
    unsigned column = static_cast<unsigned>((x - BORDER_WIDTH) / SQUARE_SIZE);
    unsigned row = static_cast<unsigned>((y - BORDER_WIDTH) / SQUARE_SIZE);
    assert(column < BOARD_SIZE);
    assert(row < BOARD_SIZE);
    stones.push_back({Team::BLACK, row, column});
    return 0;
}

int GomokuInterface::handleMouseReleased(double x, double y) {
    return 0;
}
