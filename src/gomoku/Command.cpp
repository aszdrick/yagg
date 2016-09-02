/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Command.hpp"
#include "gomoku/State.hpp"

base::Command<go::State>::Command(go::Team team,
                                  const go::Position& position,
                                  bool valid)
 : team(team), position(position), valid(valid) { }

void base::Command<go::State>::setPosition(const go::Position& pos) {
    position = pos;
    valid = true;
}

void base::Command<go::State>::execute(go::State& state) {
    if (valid) {
        state.play(position, team);
    }
}
