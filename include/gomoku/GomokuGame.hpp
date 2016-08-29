/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_GAME_HPP
#define GOMOKU_GAME_HPP

#include "GomokuState.hpp"

enum class MatchType { 
    PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
};

template<MatchType>
class GomokuFactory;

class GomokuGame {
	template<MatchType>
	friend class GomokuFactory;
 public:

    // static GomokuState makeMovement(Command);
 private:
    GomokuGame() {}
    GomokuState state;
};

#endif /* GOMOKU_GAME_HPP */
