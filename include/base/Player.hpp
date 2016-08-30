/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "InputComponent.hpp"

template<typename InputProvider>
class Player {
 public:
    void update(InputProvider& provider) {
        input.update(*this, provider);
    }
 private:
    InputComponent<Player, InputProvider> input;
};

template<typename Provider>
using PlayerInput = InputComponent<Player<Provider>, Provider>;

#endif /* PLAYER_HPP */