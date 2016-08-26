/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_GAME_HPP
#define GOMOKU_GAME_HPP

#include "GomokuState.hpp"

class GomokuGame {
 public:
    GomokuGame() = delete;

    // static GomokuState makeMovement(Command);
 private:
    GomokuState state;
};

#endif /* GOMOKU_GAME_HPP */
