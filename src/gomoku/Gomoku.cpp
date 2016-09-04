/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "gomoku/Match.hpp"
#include "gomoku/Player.hpp"

Gomoku::Gomoku()
 : Game(new Match(Player(), Player())) {

}
