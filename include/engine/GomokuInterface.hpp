/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_INTERFACE_HPP
#define GOMOKU_INTERFACE_HPP

#include <SFML/Graphics.hpp>
#include "core/GomokuGame.hpp"

class GomokuInterface {
 public:
    void draw(sf::RenderWindow&) const;
    int handleMousePressed(double, double);
    int handleMouseReleased(double, double);

 private:
    std::vector<Stone> stones;
    void drawBoard(sf::RenderWindow&) const;
    void drawBalls(sf::RenderWindow&) const;
};

#endif /* GOMOKU_INTERFACE_HPP */
