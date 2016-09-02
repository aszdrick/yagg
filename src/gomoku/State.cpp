/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/State.hpp"


void go::State::play(const go::Position& position, go::Team team) {
    stones.push_back(go::Stone{position, team});
    player = 1 - player;
}