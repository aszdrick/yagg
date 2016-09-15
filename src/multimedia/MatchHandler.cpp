/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cmath>
#include "multimedia/MatchHandler.hpp"
#include "gomoku/Traits.hpp"

Match::Handler::Product Match::Handler::doUpdate(Agent& window, Element& event) {
    auto position = MatchTraits::SUP_BOARD_LIMIAR + go::Position{1,1};
    switch(event.type) {
        case sf::Event::MouseButtonPressed: {
            auto coords = window.mapPixelToCoords({
                event.mouseButton.x, 
                event.mouseButton.y
            });
            Pixel pixel = {
                static_cast<float>(coords.x),
                static_cast<float>(coords.y)
            };
            position = pixelToPosition(pixel);
            break;
        }
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                return {Request::Type::PAUSE, position};
            }
            break;
        default:;
    }
    return {Request::Type::PLAY, position};
}

go::Position Match::Handler::pixelToPosition(const Pixel& p) {
    auto boardStart = MatchTraits::BORDER_WIDTH;
    auto squareSize = MatchTraits::SQUARE_SIZE;
    int column = round((p.x - boardStart) / squareSize);
    int row = round((p.y - boardStart) / squareSize);
    return {row, column};
}