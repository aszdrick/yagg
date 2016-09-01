/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */
#include "gomoku/PlayerInputHandler.hpp"

void gomoku::PlayerInputHandler::doUpdate(Agent& agent, Element& list) {
    for (auto& event : list) {
        agent = event;
        break;
    }
}
