/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Traits.hpp"

const go::Position GomokuTraits::INVALID_POSITION = {
    BOARD_DIMENSION, BOARD_DIMENSION
};

const sf::Color GomokuTraits::WHITE_COLOR(200, 200, 200);
const sf::Color GomokuTraits::BLACK_COLOR(50, 50, 50);

const sf::Color GomokuTraits::WHITE_OUTLINE_COLOR(100, 100, 100);
const sf::Color GomokuTraits::BLACK_OUTLINE_COLOR(100, 100, 100);

const gm::Pixel MatchTraits::INF_BOARD_LIMIAR = {
    BORDER_WIDTH/2, BORDER_WIDTH/2
};
const gm::Pixel MatchTraits::SUP_BOARD_LIMIAR = {
    2/3*BORDER_WIDTH + GomokuTraits::BOARD_DIMENSION * SQUARE_SIZE,
    2/3*BORDER_WIDTH + GomokuTraits::BOARD_DIMENSION * SQUARE_SIZE
};
