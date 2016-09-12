/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_HANDLER_HPP
#define GOMOKU_MATCH_HANDLER_HPP

#include "gomoku/Match.hpp"

class Gomoku::Match::Handler : public InputComponent {
 private:
    struct Pixel;
    void doUpdate(Agent&, Element&) override;
    go::Position pixelToPosition(const Pixel&);
};

struct Gomoku::Match::Handler::Pixel {
    float x;
    float y;
};

#endif /* GOMOKU_MATCH_HANDLER_HPP */