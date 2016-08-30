/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "core/GomokuBot.hpp"
#include "core/GomokuFactory.hpp"
#include "core/GomokuGame.hpp"
#include "engine/GameEngine.hpp"

int main(int argc, char** argv) {
    auto game = GomokuFactory<MatchType::PLAYER_VS_BOT>::create(GomokuBot());
    auto engine = GameEngine(game, 1024, 768, "Gomoku from hell");

    engine.run();

    return 0;
}
