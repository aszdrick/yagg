/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include "base/Player.hpp"
#include "mbe/Engine.hpp"
#include "mbe/GameMenu.hpp"
#include "CommonTypes.hpp"

class Gomoku : public mbe::Game {
 public:
    using PlayerInput = std::list<go::Position>;
    using BasePlayer = base::Player<go::State, PlayerInput>;
    class Match;
    class Player;
    template<unsigned>
    class AIHandler;

    Gomoku();

    void newGame();
    void optionsMenu();
    void quit();

 private:
    void switchScreenMode(Renderer&);
    void keyPressed(const sf::Event&);

    void onUpdateRenderer(Renderer&) override;
    void onProcessInput(Input&) override;
    void onSetVideoMode(Renderer&, double, double, double) override;
};

#endif /* GOMOKU_HPP */
