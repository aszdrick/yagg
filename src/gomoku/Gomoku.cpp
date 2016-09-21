/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "gomoku/Player.hpp"
#include "AI/AIHandler.hpp"
#include "AI/RandomHandler.hpp"

const std::array<Gomoku::Menu::Option, 2> Gomoku::menuOptions = {
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

const std::array<Gomoku::PauseMenu::Option, 3> Gomoku::pauseOptions = {
    Gomoku::PauseMenu::Option{
        sf::Text("Return to Game", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) { gomoku.returnToGame(); }
    },
    Gomoku::PauseMenu::Option{
        sf::Text("Return to Main Menu", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) { gomoku.returnToMainMenu(); }
    },
    Gomoku::PauseMenu::Option{
        sf::Text("Quit", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) { gomoku.quit(); }
    }
};

const std::array<Gomoku::NewGameMenu::Option, 4> Gomoku::newGameOptions = {
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
        sf::Text("Computer vs Player", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) {
            gomoku.newGame<Gomoku::AIHandler<1>, Player::InputHandler>();
        }
    },
    Gomoku::NewGameMenu::Option{
        sf::Text("Computer vs Computer", sf::Font()),
        sf::Color::Black,
        [](Gomoku& gomoku) {
            gomoku.newGame<Gomoku::AIHandler<1>, Gomoku::AIHandler<1>>();
        }
    }
};

Gomoku::Gomoku()
 : Game(new Menu(*this, menuOptions)) {

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
    pushState(new NewGameMenu(*this, newGameOptions));
}

void Gomoku::onPauseGame() {
    pushState(new PauseMenu(*this, pauseOptions));
}

void Gomoku::returnToGame() {
    popState();
}

void Gomoku::returnToMainMenu() {
    pop({State::Response::Type::POP, 2, nullptr});
}

void Gomoku::quit() {
    close();
}
