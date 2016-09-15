/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "gomoku/Player.hpp"
#include "AI/AIHandler.hpp"

const std::array<Gomoku::Menu::Option, 2> Gomoku::options = {
    Gomoku::Menu::Option{
        sf::Text("New Game", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) { gomoku.newGameMenu(); }
    },
    Gomoku::Menu::Option{
        sf::Text("Quit", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) { gomoku.quit(); }
    }
};

const std::array<Gomoku::NewGameMenu::Option, 3> Gomoku::subOptions = {
    Gomoku::NewGameMenu::Option{
        sf::Text("Player vs Player", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) {
            gomoku.newGame<Player::InputHandler>();
        }
    },
    Gomoku::NewGameMenu::Option{
        sf::Text("Player vs Computer", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) {
            gomoku.newGame<Player::InputHandler, Gomoku::AIHandler<1>>();
        }
    },
    Gomoku::NewGameMenu::Option{
        sf::Text("Computer vs Computer", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) {
            gomoku.newGame<Gomoku::AIHandler<1>>();
        }
    }
};

Gomoku::Gomoku()
 : Game(new Menu(*this, options)) {

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

void Gomoku::newGameMenu() {
    pushState(new NewGameMenu(*this, subOptions));
}

void Gomoku::quit() {
    close();
}
