/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "AI/MiniMaxTree.hpp"
#include "gomoku/State.hpp"

#include "gomoku/Gomoku.hpp"
#include "mbe/Engine.hpp"

int main(int argc, char** argv) {
    auto engine = mbe::Engine(new Gomoku(),"Yet Another Gomoku Game", 1024, 768);
    engine.run();

    auto fn = [](const go::State&) {
        return 0;
    };
    MiniMaxTree<go::State> tree(fn, fn);

    go::State state;
    tree.analyze(state);

    return 0;
}
