/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_STATE_HPP
#define GOMOKU_STATE_HPP

#include "extra/Matrix.hpp"

namespace gomoku {
    class State {
     public:

     private:
        Matrix<15,15> table;
    };
}

#endif /* GOMOKU_STATE_HPP */