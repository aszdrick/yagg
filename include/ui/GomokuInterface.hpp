/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_INTERFACE_HPP
#define GOMOKU_INTERFACE_HPP

#include <SFML/Graphics.hpp>
#include "gomoku/GomokuGame.hpp"

class GomokuInterface {
 public:
    void draw(sf::RenderWindow&) const;
    int handleMousePressed(double, double);
    int handleMouseReleased(double, double);

 private:
    const float BOARD_SIZE = 15;
    const float SQUARE_SIZE = 48;
    const float BORDER_WIDTH = SQUARE_SIZE;
    std::vector<Stone> stones;
    const static sf::Color WHITE_COLOR;
    const static sf::Color BLACK_COLOR;

    void drawBoard(sf::RenderWindow&) const;
    void drawBalls(sf::RenderWindow&) const;
};

#endif /* GOMOKU_INTERFACE_HPP */
