/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_INPUT_HPP
#define GOMOKU_MATCH_INPUT_HPP

#include "Match.hpp"

namespace gomoku {
    class MatchInput : public Match::InputHandler {
     private:
        void doUpdate(const Agent&, Element&);
    };
}

#endif /* GOMOKU_MATCH_INPUT_HPP */
