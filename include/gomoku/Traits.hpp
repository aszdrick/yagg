/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "gomoku/CommonTypes.hpp"
#include "Traits.hpp"

template<typename T>
struct Traits;

class Gomoku;

template<>
struct Traits<Gomoku> {
    constexpr static unsigned BOARD_DIMENSION = 15;
    constexpr static float SQUARE_SIZE = 48;
    constexpr static float BORDER_WIDTH = SQUARE_SIZE;
    constexpr static unsigned STONE_RADIUS = 18;
    constexpr static unsigned STONE_BORDER_WIDTH = 2; 
    const static go::Position INVALID_POSITION;
    const static sf::Color WHITE_COLOR;
    const static sf::Color BLACK_COLOR;
    const static sf::Color WHITE_OUTLINE_COLOR;
    const static sf::Color BLACK_OUTLINE_COLOR;
};

using GomokuTraits = Traits<Gomoku>;

#endif /* TRAITS_HPP */
