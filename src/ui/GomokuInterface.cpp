/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "ui/GomokuInterface.hpp"

void GomokuInterface::draw(sf::RenderWindow& window) const {
    sf::CircleShape circle(50);
    window.draw(circle);
}

int GomokuInterface::handleMousePressed(double x, double y) {
    return 0;
}

int GomokuInterface::handleMouseReleased(double x, double y) {
    return 0;
}
