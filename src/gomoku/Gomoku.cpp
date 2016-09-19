/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "gomoku/Player.hpp"
#include "AI/AIHandler.hpp"
#include "AI/RandomHandler.hpp"

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
            // gomoku.newGame<Player::InputHandler, Gomoku::AIHandler<2>>();
            gomoku.newGame<Gomoku::AIHandler<2>, Player::InputHandler>();
        }
    },
    Gomoku::NewGameMenu::Option{
        sf::Text("Computer vs Computer", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) {
            gomoku.newGame<Gomoku::AIHandler<4>, Gomoku::AIHandler<6>>();
        }
    }
};

Gomoku::Gomoku()
 : Game(new Menu(*this, options)) {

}

void Gomoku::onProcessInput(InputProcessor& processor, Input& events) {
    for (auto event : events) {
        if (event.type == sf::Event::KeyPressed) {
            keyPressed(event);
        }
    }
}

void Gomoku::keyPressed(const sf::Event& event) {
    if (event.key.code == sf::Keyboard::F12) {
        Game::switchScreenMode();
    }
}

void Gomoku::newGameMenu() {
    pushState(new NewGameMenu(*this, subOptions));
}

void Gomoku::quit() {
    close();
}
