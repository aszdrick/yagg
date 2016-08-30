/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "core/GomokuGame.hpp"

void GomokuGame::update(sf::RenderWindow& window) {
    graphics.update(*this, window);
    players[0].update(window);
}