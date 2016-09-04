/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/State.hpp"
#include "extra/macros.hpp"

void go::State::play(const go::Position& position, go::Team team) {
    // stones.push_back(go::Stone{position, team});
    if (!analyzer.isOccupied(position)) {
        // board[position.row][position.column] = go::Stone{position, team};
        analyzer.play(position, team);
        player = 1 - player;
        TRACE(isOver());
    }
}

void go::State::iterate(const std::function<void(const go::Stone&)>& fn) const {
    analyzer.iterate(fn);
}

short go::State::currentPlayer() const {
    return player;
}


bool go::State::isOver() const {
    return analyzer.isOver();
}
