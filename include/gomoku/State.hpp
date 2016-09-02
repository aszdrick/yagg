/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GO_STATE_HPP
#define GO_STATE_HPP

#include "CommonTypes.hpp"

class go::State {
 public:
    short player = 0;
    std::list<Stone> stones;

    void play(const go::Position&, go::Team);
};

#endif /* GO_STATE_HPP */
