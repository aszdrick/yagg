/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "core/Controller.hpp"
#include "gomoku/GomokuGame.hpp"
#include "gomoku/GomokuFactory.hpp"
#include "ui/Interface.hpp"
#include "macros.hpp"

int main(int argc, char** argv) {
    Controller controller;
    signals::init(&argc, &argv);
    signals::set_controller(&controller);
    signals::start();

    // auto game = GomokuFactory<MatchType::PLAYERS_ONLY>::create();

    return 0;
}