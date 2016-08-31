/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_GRAPHICS_HPP
#define GOMOKU_MATCH_GRAPHICS_HPP

#include "Match.hpp"

namespace gomoku {
    class MatchGraphics : public Match::Graphics {
     private:
        void drawBoard(Element&) const;
        void doUpdate(const Agent&, Element&);
    };
}

#endif /* GOMOKU_MATCH_GRAPHICS_HPP */
