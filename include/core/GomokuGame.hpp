/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_GAME_HPP
#define GOMOKU_GAME_HPP

#include "base/Game.hpp"
#include "GomokuPlayer.hpp"
#include "GomokuState.hpp"
#include "engine/GameGraphics.hpp"


enum class MatchType { 
    PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
};

enum class Team {
    BLACK, WHITE
};

template<MatchType>
class GomokuFactory;

class GomokuGame : public Game<sf::RenderWindow> {
	template<MatchType>
	friend class GomokuFactory;
 public:
    void update(sf::RenderWindow&);
 private:
    GomokuGame() {}

    GomokuState state;
    GameGraphics graphics;
    GomokuPlayer players[2];
};

struct Stone {
    Team team;
    unsigned row;
    unsigned column;
};

#endif /* GOMOKU_GAME_HPP */
