/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cmath>
#include "multimedia/MatchHandler.hpp"
#include "gomoku/Traits.hpp"

void Gomoku::Match::Handler::doUpdate(Agent& match, Element& list) {
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
                
                if (!match.isOver() && isInsideBoard(pixel)) {
                    auto position = pixelToPosition(pixel);
                    inputs.push_back(position);                    
                }

                break;
            }
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                    case sf::Keyboard::F1:
                        match.restart();
                        break;
                    default:;
                }
                break;
            default:;
        }
    }

    if (!inputs.empty()) {
        match.updatePlayers(inputs);
    }
}

bool Gomoku::Match::Handler::isInsideBoard(const gm::Pixel& pixel) {
    return pixel >= MatchTraits::INF_BOARD_LIMIAR
        && pixel <= MatchTraits::SUP_BOARD_LIMIAR;
}

go::Position Gomoku::Match::Handler::pixelToPosition(const gm::Pixel& p) {
    auto boardStart = MatchTraits::BORDER_WIDTH;
    auto squareSize = MatchTraits::SQUARE_SIZE;
    int column = round((p.x - boardStart) / squareSize);
    int row = round((p.y - boardStart) / squareSize);
    return {row, column};
}