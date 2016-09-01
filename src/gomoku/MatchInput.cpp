/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <cmath>
#include <queue>
#include "gomoku/MatchInput.hpp"
#include "GomokuTraits.hpp"
#include "macros.hpp"

namespace gomoku {
    void MatchInput::doUpdate(Agent& agent, Element& list) {
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

        agent.handleEvents(inputs);
    }

    gomoku::Game::PlayerInput MatchInput::handleMousePressed(float x, float y) {
        auto boardStart = GomokuTraits::BORDER_WIDTH;
        auto squareSize = GomokuTraits::SQUARE_SIZE;
        unsigned column = round((x - boardStart) / squareSize);
        unsigned row = round((y - boardStart) / squareSize);
        return {row, column};
    }
}

