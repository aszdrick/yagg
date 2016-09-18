/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/State.hpp"
#include "gomoku/StateGenerator.hpp"

#include "AI/MiniMaxTree.hpp"
#include "gomoku/State.hpp"

#include "gomoku/Gomoku.hpp"
#include "mbe/Engine.hpp"

int main(int argc, char** argv) {
    auto engine = mbe::Engine(new Gomoku(),
        "Yet Another Gomoku Game",
        GomokuTraits::WINDOW_WIDTH,
        GomokuTraits::WINDOW_HEIGHT
    );
    engine.run();

    // go::State state;
    // TRACE_IT(state.possibleSquares());
    // BLANK
    // state.play(go::Position{7, 7}, go::Team::BLACK);
    // TRACE_IT(state.possibleSquares());
    // BLANK
    // state.play(go::Position{9, 8}, go::Team::WHITE);
    // TRACE_IT(state.possibleSquares());
    // BLANK
    // state.undo();
    // TRACE_IT(state.possibleSquares());

    // auto fn = [](const go::State&) {
    //     return 0;
    // };
    // MiniMaxTree<go::State> tree(fn, fn);

    // go::State state;
    // tree.analyze(state);

    return 0;
}
