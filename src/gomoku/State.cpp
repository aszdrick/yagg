/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/State.hpp"
#include "extra/macros.hpp"

void go::State::play(const go::Position& position, go::Team team) {
    if (!analyzer.occupied(position)) {
        // ranger.play(position, team);
        analyzer.play(position, team);

        if (analyzer.hasWinner()) {
            winner = player;
        }

        // if (ranger.finished() && !ranger.tie()) {
        //     winner = player;
        // }

        player = 1 - player;
    }
}
