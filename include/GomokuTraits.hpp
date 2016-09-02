/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_TRAITS_HPP
#define GOMOKU_TRAITS_HPP

#include <SFML/Graphics.hpp>
#include "Traits.hpp"

class GomokuGame;

template<>
struct Traits<GomokuGame> {
    constexpr static unsigned BOARD_DIMENSION = 15;
    constexpr static float SQUARE_SIZE = 48;
    constexpr static float BORDER_WIDTH = SQUARE_SIZE;
    constexpr static unsigned STONE_RADIUS = 24;
    const static sf::Color WHITE_COLOR;
    const static sf::Color BLACK_COLOR;
};

using GomokuTraits = Traits<GomokuGame>;

const sf::Color GomokuTraits::WHITE_COLOR(128, 128, 128);
const sf::Color GomokuTraits::BLACK_COLOR(102, 51, 0);

#endif /* GOMOKU_TRAITS_HPP */
