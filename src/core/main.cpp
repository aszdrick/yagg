/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/GomokuBot.hpp"
#include "gomoku/GomokuFactory.hpp"
#include "gomoku/GomokuGame.hpp"
#include "ui/GomokuInterface.hpp"
#include "macros.hpp"

#include "ui/GameEngine.hpp"

template<typename G, typename I>
GameEngine<G,I> make_gomoku(G& game, I& interface, double width,
    double height, const std::string& title) {

    return GameEngine<G,I>(game, interface, width, height, title);
}

int main(int argc, char** argv) {
    // GameEngine<GomokuGame, GomokuInterface> game(800, 600, "Gomoku from hell");
    auto game = GomokuFactory<MatchType::PLAYER_VS_BOT>::create(GomokuBot());
    GomokuInterface interface;
    auto engine = make_gomoku(game, interface, 800, 600, "Gomoku from hell");

    engine.run();

    return 0;
}
