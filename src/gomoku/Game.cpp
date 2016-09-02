/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Game.hpp"
#include "gomoku/Match.hpp"
#include "gomoku/Player.hpp"

gomoku::Game::Game()
 : mbe::Game(new Match(gomoku::Player(), gomoku::Player())) {

}
