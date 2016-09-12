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
    class AIHandler;

    Gomoku();

    State* newGame();
    State* optionsMenu();
    State* quit();
};

#endif /* GOMOKU_HPP */
