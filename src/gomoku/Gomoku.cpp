/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "gomoku/Match.hpp"
#include "gomoku/Player.hpp"
#include "AI/AIHandler.hpp"

Gomoku::Gomoku()
 : Game(new mbe::GameMenu<Gomoku>(*this)) {

}

void Gomoku::onSetVideoMode(Renderer& window, double w, double h, double d) {

}

void Gomoku::onUpdateRenderer(Renderer& renderer) {

}

void Gomoku::onProcessInput(Input& events) {
    for (auto event : events) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                keyPressed(event);
                break;
            default:;
        }
    }
}

void Gomoku::keyPressed(const sf::Event& event) {
    switch (event.key.code) {
        case sf::Keyboard::F12: {
            Game::switchScreenMode();
            break;
        }
        default:;
    }
}

void Gomoku::newGame() {
    pushState(new Match(Player(new AIHandler<1>()), Player(new AIHandler<2>())));
    // pushState(new Match(Player(), Player()));
}

void Gomoku::optionsMenu() {
    
}

void Gomoku::quit() {
    close();
}
