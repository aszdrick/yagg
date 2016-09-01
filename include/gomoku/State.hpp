/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_STATE_HPP
#define GOMOKU_STATE_HPP

#include "extra/Matrix.hpp"
#include "gomoku/Player.hpp"

namespace gomoku {
    enum class Team {
        BLACK, WHITE
    };

    struct Stone {
        Team team;
        unsigned row;
        unsigned column;
    };

    class State {
     public:
        void addStone(const Point&, Team);
        const std::list<Stone>& getStones() const {
            return stones;
        }

     private:
        // Matrix<15, 15> table;
        std::list<Stone> stones;
    };
}

#endif /* GOMOKU_STATE_HPP */
