/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Traits.hpp"

const go::Position GomokuTraits::INVALID_POSITION = {
    BOARD_DIMENSION, BOARD_DIMENSION
};

const sf::Color GomokuTraits::WHITE_COLOR(200, 200, 200);
const sf::Color GomokuTraits::BLACK_COLOR(50, 50, 50);

const sf::Color GomokuTraits::WHITE_OUTLINE_COLOR(100, 100, 100);
const sf::Color GomokuTraits::BLACK_OUTLINE_COLOR(100, 100, 100);