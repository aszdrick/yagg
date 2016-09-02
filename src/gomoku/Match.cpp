/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <cmath>
#include <type_traits>
#include <utility>
#include "gomoku/Match.hpp"
#include "GomokuTraits.hpp"

//------------------------------- Match class -------------------------------//

gomoku::Match::Match(Player&& p1, Player&& p2,
        Graphics* const graphics, InputHandler* const input)
: graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
  graphics(*graphicsPtr), input(*inputPtr),
  players{std::move(p1), std::move(p2)} { }

void gomoku::Match::handleEvents(Player::Input& events) {
    players[0].processEvents(events);
    auto move = players[0].pendingMove();
    if (move.valid) {
        state.addStone(move.coords, order[currentPlayer]);
        currentPlayer = 1 - currentPlayer;
    }
}

void gomoku::Match::onUpdateRenderer(Renderer& render) {
    graphics.update(*this, render);
}

gomoku::Match::Transition gomoku::Match::onProcessInput(Input& in) {
    input.update(*this, in);
    return { Transition::Type::SELF, this};
}

//-------------------------- Match::Graphics class --------------------------//

void gomoku::Match::Graphics::doUpdate(Agent& match, Element& window) {
    drawBoard(window);
    drawBalls(match, window);
}

void gomoku::Match::Graphics::drawBoard(Element& window) const {
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

void gomoku::Match::Graphics::drawBalls(Agent& match, Element& window) const {
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

//------------------------ Match::InputHandler class ------------------------//

void gomoku::Match::InputHandler::doUpdate(Agent& match, Element& list) {
    std::list<Game::PlayerInput> inputs;
    while (!list.empty()) {
        auto event = list.front();
        list.pop_front();

        switch(event.type) {
            case sf::Event::MouseButtonPressed: {
                float x = event.mouseButton.x;
                float y = event.mouseButton.y;
                auto pair = handleMousePressed(x, y);
                int boardDimension = GomokuTraits::BOARD_DIMENSION;
                if (pair.first >= 0 && pair.second >= 0
                    && pair.first < boardDimension
                    && pair.second < boardDimension) {
                    
                    inputs.push_back(handleMousePressed(x, y));
                }
                break;
            }
            default:;
        }
    }

    match.handleEvents(inputs);
}

gomoku::Game::PlayerInput gomoku::Match::InputHandler::handleMousePressed(float x, float y) {
    auto boardStart = GomokuTraits::BORDER_WIDTH;
    auto squareSize = GomokuTraits::SQUARE_SIZE;
    unsigned column = round((x - boardStart) / squareSize);
    unsigned row = round((y - boardStart) / squareSize);
    return {row, column};
}
