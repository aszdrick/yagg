// /* Copyright 2016 Ghabriel Nunes, Marleson Graf
//    <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

// #ifndef GOMOKU_FACTORY_HPP
// #define GOMOKU_FACTORY_HPP

// #include "GomokuGame.hpp"
// #include "Bot.hpp"

// template<MatchType Type>
// struct GomokuFactory { };

// template<>
// struct GomokuFactory<MatchType::PLAYER_VS_BOT> {
//     static GomokuGame create(const Bot<GomokuState>&) {
//         return GomokuGame();
//     }
// };

// template<>
// struct GomokuFactory<MatchType::PLAYERS_ONLY> {
//     static GomokuGame create() {
//         return GomokuGame();
//     }
// };

// template<>
// struct GomokuFactory<MatchType::BOTS_ONLY> {
//     static GomokuGame create(const Bot<GomokuState>&, const Bot<GomokuState>&) {
//         return GomokuGame();
//     }
// };

// #endif /* GOMOKU_FACTORY_HPP */
