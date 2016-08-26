/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_FACTORY_HPP
#define GOMOKU_FACTORY_HPP

#include "GomokuGame.hpp"
#include "core/Bot.hpp"

enum class MatchType { 
    PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
};

template<MatchType Type>
struct GomokuFactory { };

template<>
struct GomokuFactory<MatchType::PLAYER_VS_BOT> {
    static GomokuGame create(const Bot<GomokuState>&);
};

template<>
struct GomokuFactory<MatchType::PLAYERS_ONLY> {
    static GomokuGame create();
};

template<>
struct GomokuFactory<MatchType::BOTS_ONLY> {
    static GomokuGame create(const Bot<GomokuState>&, const Bot<GomokuState>&);
};

#endif /* GOMOKU_FACTORY_HPP */
