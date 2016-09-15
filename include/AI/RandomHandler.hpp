/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef RANDOM_HANDLER_HPP
#define RANDOM_HANDLER_HPP

#include "gomoku/Player.hpp"

class RandomHandler : public Player::InputComponent {
 public:
    RandomHandler();

 private:
    Product doUpdate(Agent&, Element&) override;
};

#endif /* RANDOM_HANDLER_HPP */
