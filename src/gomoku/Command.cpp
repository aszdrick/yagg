/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Command.hpp"
#include "gomoku/State.hpp"

base::Command<go::State>::Command(go::Team team, const go::Position& position)
 : team(team), position(position) { }

void base::Command<go::State>::setPosition(const go::Position& pos) {
    position = pos;
}

void base::Command<go::State>::execute(go::State& state) {
    state.play(position, team);
}
