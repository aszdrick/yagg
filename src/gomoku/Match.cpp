/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <cmath>
#include <type_traits>
#include <utility>
#include "gomoku/Traits.hpp"
#include "gomoku/Match.hpp"
#include "extra/macros.hpp"

//------------------------------- Match class -------------------------------//

Gomoku::Match::Match(Player&& p1, Player&& p2,
        Graphics* const graphics, InputHandler* const input)
: graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
  graphics(*graphicsPtr), input(*inputPtr),
  players{std::move(p1), std::move(p2)} {
    players[0].setTeam(go::Team::BLACK);
    players[1].setTeam(go::Team::WHITE);
}

void Gomoku::Match::updatePlayers(Player::Input& positions) {
    auto move = players[state.currentPlayer()].processInput(state, positions);
    move.execute(state);
}

void Gomoku::Match::onUpdateRenderer(Renderer& render) {
    graphics.update(*this, render);
}

Gomoku::Match::Transition Gomoku::Match::onProcessInput(Input& in) {
    input.update(*this, in);
    return { Transition::Type::SELF, this};
}

//------------------------ Match::InputHandler class ------------------------//

void Gomoku::Match::InputHandler::doUpdate(Agent& match, Element& list) {
    std::list<go::Position> inputs;
    while (!list.empty()) {
        auto event = list.front();
        list.pop_front();

        switch(event.type) {
            case sf::Event::MouseButtonPressed: {
                gm::Pixel pixel = {
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                };
                
                if (isInsideBoard(pixel)) {
                    auto position = pixelToPosition(pixel);
                    inputs.push_back(position);                    
                }

                break;
            }
            default:;
        }
    }

    match.updatePlayers(inputs);
}

bool Gomoku::Match::InputHandler::isInsideBoard(const gm::Pixel& pixel) {
    return pixel >= MatchTraits::INF_BOARD_LIMIAR
        || pixel <= MatchTraits::SUP_BOARD_LIMIAR;
}

go::Position Gomoku::Match::InputHandler::pixelToPosition(const gm::Pixel& p) {
    auto boardStart = MatchTraits::BORDER_WIDTH;
    auto squareSize = MatchTraits::SQUARE_SIZE;
    unsigned column = round((p.x - boardStart) / squareSize);
    unsigned row = round((p.y - boardStart) / squareSize);
    return {row, column};
}

//-------------------------- Match::Graphics class --------------------------//

void Gomoku::Match::Graphics::doUpdate(Agent& match, Element& window) {
    drawBoard(window);
    drawBalls(match, window);
}

void Gomoku::Match::Graphics::drawBoard(Element& window) const {
    static std::vector<sf::Vertex> lines;
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    if (lines.empty()) {
        auto squareSize = MatchTraits::SQUARE_SIZE;
        auto boardStart = MatchTraits::BORDER_WIDTH;
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

void Gomoku::Match::Graphics::drawBalls(Agent& match, Element& window) const {
    match.state.iterate([&window](auto& stone) {
        auto shape = sf::CircleShape(MatchTraits::STONE_RADIUS, 50);
        auto squareSize = MatchTraits::SQUARE_SIZE;
        shape.setPosition(sf::Vector2f(
            squareSize + stone.position.column * squareSize 
            - MatchTraits::STONE_RADIUS,
            squareSize + stone.position.row * squareSize 
            - MatchTraits::STONE_RADIUS));

        auto white = GomokuTraits::WHITE_COLOR;
        auto black = GomokuTraits::BLACK_COLOR;
        auto w_border = GomokuTraits::WHITE_OUTLINE_COLOR;
        auto b_border = GomokuTraits::BLACK_OUTLINE_COLOR;

        shape.setOutlineThickness(MatchTraits::STONE_BORDER_WIDTH);
        
        shape.setOutlineColor(stone.team == go::Team::WHITE ? w_border : b_border);

        shape.setFillColor(stone.team == go::Team::WHITE ? white : black);
        window.draw(shape);
    });
}