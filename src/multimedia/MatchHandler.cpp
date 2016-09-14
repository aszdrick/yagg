/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cmath>
#include "multimedia/MatchHandler.hpp"
#include "gomoku/Traits.hpp"

void Match::Handler::doUpdate(Agent& match, Element& list) {
    std::list<go::Position> inputs;
    while (!list.empty()) {
        auto event = list.front();
        list.pop_front();

        switch(event.type) {
            case sf::Event::MouseButtonPressed: {
                Pixel pixel = {
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                };

                auto position = pixelToPosition(pixel);
                inputs.push_back(position);

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

    match.updatePlayers(inputs);
}

go::Position Match::Handler::pixelToPosition(const Pixel& p) {
    auto boardStart = MatchTraits::BORDER_WIDTH;
    auto squareSize = MatchTraits::SQUARE_SIZE;
    int column = round((p.x - boardStart) / squareSize);
    int row = round((p.y - boardStart) / squareSize);
    return {row, column};
}