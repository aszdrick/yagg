/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include "CommonTypes.hpp"
#include "gomoku/Match.hpp"
#include "mbe/Engine.hpp"
#include "mbe/Menu.hpp"

class Gomoku : public mbe::Game {
 public:
    using Menu = mbe::Menu<Gomoku, 2>;
    using NewGameMenu = mbe::Menu<Gomoku, 4>;
    using PauseMenu = mbe::Menu<Gomoku, 3>;
    template<unsigned>
    class AIHandler;

    Gomoku();

    template<typename BH, typename WH = BH>
    inline void newGame() {
        popState();
        pushState(new Match(Player(new BH()), Player(new WH())));
    }

    void quit();

 private:
    static const std::array<Menu::Option, 2> menuOptions;
    static const std::array<NewGameMenu::Option, 4> newGameOptions;
    static const std::array<PauseMenu::Option, 3> pauseOptions;

    const auto desiredOptions();
    const auto desiredSubOptions();

    void newGameMenu();
    void returnToGame();
    void returnToMainMenu();

    void switchScreenMode(Renderer&);
    void keyPressed(const sf::Event&);

    void onPauseGame() override;
    void onProcessInput(InputProcessor&, Input&) override;
};

#endif /* GOMOKU_HPP */
