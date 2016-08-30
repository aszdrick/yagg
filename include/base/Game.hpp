/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"

template <typename Renderer>
class Game {
 public:
    virtual void update(Renderer&) = 0;
};

#endif /* GAME_HPP */