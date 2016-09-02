/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_COMMAND_HPP
#define GOMOKU_COMMAND_HPP

#include "base/Command.hpp"
#include "CommonTypes.hpp"

namespace base {
    template<>
    class Command<go::Board> {
     public:
        void execute(go::Board&);
     private:
        go::Position position;
        bool valid;
    };
}

#endif /* GOMOKU_COMMAND_HPP */