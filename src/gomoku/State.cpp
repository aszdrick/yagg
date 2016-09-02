/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/State.hpp"

void go::State::play(const go::Position& position, go::Team team) {
    // stones.push_back(go::Stone{position, team});
    if (!isOccupied(position)) {
        board[position.row][position.column] = go::Stone{position, team};
        player = 1 - player;
    }
}

void go::State::iterate(const std::function<void(const go::Stone&)>& fn) const {
    for (auto& row : board) {
        for (auto& pair : row.second) {
            fn(pair.second);
        }
    }
}

short go::State::currentPlayer() const {
    return player;
}

bool go::State::isOccupied(const go::Position& position) const {
    return board.count(position.row)
        && board.at(position.row).count(position.column);
}
