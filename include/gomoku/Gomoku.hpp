/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_HPP
#define GOMOKU_HPP

#include "base/Player.hpp"
#include "mbe/Engine.hpp"
#include "CommonTypes.hpp"

class Gomoku : public mbe::Game {
 public:
    using BasePlayer = base::Player<Game::Renderer, std::list<go::Position>>;
    class Match;
    class Player;

    Gomoku();
 private:
    // void onUpdateRenderer(Renderer&) override;
    // void onProcessInput(Input&) override;
};

#endif /* GOMOKU_HPP */
