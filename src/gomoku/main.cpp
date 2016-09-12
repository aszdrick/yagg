/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "mbe/Engine.hpp"
#include "mbe/GameMenu.hpp"

int main(int argc, char** argv) {
    auto engine = mbe::Engine(new Gomoku(),
        1024, 768, "Gomoku from hell");

    engine.run();

    return 0;
}
