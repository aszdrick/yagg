/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Command.hpp"
#include "gomoku/State.hpp"
#include "gomoku/Traits.hpp"

base::Command<go::State>::Command(go::Team team)
 : team(team) { }

void base::Command<go::State>::setTeam(const go::Team& t) {
    team = t;
}

void base::Command<go::State>::setPosition(const go::Position& pos) {
    constexpr static int boardDimension = GomokuTraits::BOARD_DIMENSION;
    position = pos;
    valid = (position.row >= 0 && position.row < boardDimension
          && position.column >= 0 && position.column < boardDimension);
}

bool base::Command<go::State>::isValid() const {
    return valid;
}

void base::Command<go::State>::execute(go::State& state) {
    state.play(position, team);
}

void base::Command<go::State>::setIterations(unsigned iterations) {
    commandIterations = iterations;
}

unsigned base::Command<go::State>::iterations() const {
    return commandIterations;
}
