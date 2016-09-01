/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */
#include "gomoku/State.hpp"

void gomoku::State::addStone(const Point& point, Team team) {
    stones.push_back({team, point.first, point.second});
}
