/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "mbe/Engine.hpp"

int main(int argc, char** argv) {
    auto engine = mbe::Engine(new Gomoku(),
        1024, 768, "Gomoku from hell");

    engine.run();

    return 0;
}
