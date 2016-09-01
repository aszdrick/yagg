/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Game.hpp"
#include "mbe/Engine.hpp"
#include "gomoku/MatchGraphics.hpp"

int main(int argc, char** argv) {
    auto engine = mbe::Engine(new gomoku::Game(),
        1024, 768, "Gomoku from hell");

    engine.run();

    return 0;
}
