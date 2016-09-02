/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include "base/Player.hpp"
#include "mbe/Engine.hpp"
#include "CommonTypes.hpp"

class Gomoku : public mbe::Game {
 public:
    using PlayerInput = std::list<go::Position>;
    using BasePlayer = base::Player<go::State, Game::Renderer, PlayerInput>;
    class Match;
    class Player;

    Gomoku();
 private:
    // void onPeriodicUpdate() override;
    // void onUpdateRenderer(Renderer&) override;
    // void onProcessInput(Input&) override;
};

#endif /* GOMOKU_HPP */
