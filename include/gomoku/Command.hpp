/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_COMMAND_HPP
#define GOMOKU_COMMAND_HPP

#include "base/Command.hpp"
#include "CommonTypes.hpp"

namespace base {
    template<>
    class Command<go::State> {
     public:
        Command(go::Team = go::Team::BLACK,
                const go::Position& = {0, 0},
                bool = false);

        void setPosition(const go::Position&);
        void execute(go::State&);
     private:
        go::Team team;
        go::Position position;
        bool valid;
    };
}

#endif /* GOMOKU_COMMAND_HPP */