/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Gomoku.hpp"
#include "gomoku/Match.hpp"
#include "gomoku/Player.hpp"
#include "AI/AIHandler.hpp"

// Gomoku::Gomoku()
//  : Game(new Match(Player(), Player())) {

// }


Gomoku::Gomoku()
 : Game(new mbe::GameMenu<Gomoku>(*this)) {

}

void Gomoku::newGame() {
    pushState(new Match(Player(new AIHandler()), Player(new AIHandler())));
}

void Gomoku::optionsMenu() {
    
}

void Gomoku::quit() {
    close();
}

