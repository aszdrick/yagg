/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Component.hpp"

template<typename InputProvider>
class Player {
 public:
    using Input = Component<Player<InputProvider>, InputProvider>;
    
    void update(InputProvider& provider) {
        input.update(*this, provider);
    }
 private:
    Component<Player, InputProvider> input;
};

#endif /* PLAYER_HPP */