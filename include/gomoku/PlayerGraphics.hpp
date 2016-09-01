/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_PLAYER_GRAPHICS_HPP
#define GOMOKU_PLAYER_GRAPHICS_HPP

#include "gomoku/Player.hpp"

namespace gomoku {
    class PlayerGraphics : public Player::Graphics {
     private:
        void doUpdate(Agent&, Element&);
    };
}

#endif /* GOMOKU_PLAYER_GRAPHICS_HPP */
