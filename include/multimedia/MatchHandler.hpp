/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_HANDLER_HPP
#define GOMOKU_MATCH_HANDLER_HPP

#include "gomoku/Match.hpp"

class Gomoku::Match::Handler : public InputComponent {
 private:
    void doUpdate(Agent&, Element&) override;
    go::Position pixelToPosition(const gm::Pixel&);
    bool isInsideBoard(const gm::Pixel&);
};

#endif /* GOMOKU_MATCH_HANDLER_HPP */