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
    position = pos;
    valid = (position >= MatchTraits::INF_BOARD_LIMIAR
             && position <= MatchTraits::SUP_BOARD_LIMIAR);
}

bool base::Command<go::State>::isValid() const {
    return valid;
}

void base::Command<go::State>::execute(go::State& state) {
    state.play(position, team);
}
