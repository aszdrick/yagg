/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_GAME_HPP
#define GOMOKU_GAME_HPP

#include "mbe/Engine.hpp"

enum class MatchType { 
    PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
};

enum class Team {
    BLACK, WHITE
};

namespace gomoku {
    class Game : public mbe::Game {
     public:
        Game();
     private:
        void update() override;
        void updateGraphics(Renderer&) override;
        void processInput(Input&) override;
    };    
}

struct Stone {
    Team team;
    unsigned row;
    unsigned column;
};

#endif /* GOMOKU_GAME_HPP */
