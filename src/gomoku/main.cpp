/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "AI/MiniMaxTree.hpp"
#include "gomoku/Gomoku.hpp"
#include "gomoku/State.hpp"
#include "gomoku/StateGenerator.hpp"
#include "mbe/Engine.hpp"

int main(int argc, char** argv) {
    auto engine = mbe::Engine(new Gomoku(),
        "Yet Another Gomoku Game",
        GomokuTraits::WINDOW_WIDTH,
        GomokuTraits::WINDOW_HEIGHT
    );
    engine.run();

    return 0;
}
