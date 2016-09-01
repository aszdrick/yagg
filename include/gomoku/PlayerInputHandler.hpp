/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_PLAYER_INPUT_HANDLER_HPP
#define GOMOKU_PLAYER_INPUT_HANDLER_HPP

#include "gomoku/Player.hpp"

namespace gomoku {
    class PlayerInputHandler : public Player::InputHandler {
     private:
        void doUpdate(Agent&, Element&);
    };
}

#endif /* GOMOKU_PLAYER_INPUT_HANDLER_HPP */
