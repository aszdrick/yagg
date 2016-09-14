/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/State.hpp"
#include "extra/macros.hpp"

void go::State::play(const go::Position& position, go::Team team) {
    if (!analyzer.occupied(position)) {
        // board[position.row][position.column] = go::Stone{position, team};
        analyzer.play(position, team);
        // ivBoard.play(position, team);

        if (analyzer.hasWinner()) {
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

bool go::State::over() const {
    return analyzer.over();
}

bool go::State::hasWinner() const {
    return analyzer.hasWinner();
}

bool go::State::full() const {
    return analyzer.full();
}

void go::State::quadrupletIteration(const BoardAnalyzer::SequenceCallback& fn) const {
    analyzer.quadrupletIteration(fn);
}

bool go::State::occupied(const go::Position& position) const {
    return analyzer.occupied(position);
    // return true;
}
