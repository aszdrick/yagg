/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/State.hpp"
#include "extra/macros.hpp"

void go::State::play(const go::Position& position, go::Team team) {
    if (!analyzer.isOccupied(position)) {
        // board[position.row][position.column] = go::Stone{position, team};
        hue.play(position, team);
        analyzer.play(position, team);

        if (analyzer.isOver()) {
            winner = player;
        }

        player = 1 - player;
    }
}

void go::State::iterate(const std::function<void(const go::Stone&)>& fn) const {
    analyzer.iterate(fn);
}

short go::State::currentPlayer() const {
    return player;
}

short go::State::winnerPlayer() const {
    return winner;
}

bool go::State::isOver() const {
    return analyzer.isOver();
}

void go::State::quadrupletIteration(const BoardAnalyzer::SequenceCallback& fn) const {
    analyzer.quadrupletIteration(fn);
}
