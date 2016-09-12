/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Traits.hpp"

const sf::Color GomokuTraits::WHITE_COLOR(200, 200, 200);
const sf::Color GomokuTraits::BLACK_COLOR(50, 50, 50);
const sf::Color GomokuTraits::HIGHLIGHT_COLOR(180, 25, 25);

const sf::Color GomokuTraits::WHITE_OUTLINE_COLOR(100, 100, 100);
const sf::Color GomokuTraits::BLACK_OUTLINE_COLOR(100, 100, 100);
const sf::Color GomokuTraits::HIGHLIGHT_OUTLINE_COLOR(100, 0, 0);

const go::Position MatchTraits::INF_BOARD_LIMIAR = {
    0, 0
};
const go::Position MatchTraits::SUP_BOARD_LIMIAR = {
    GomokuTraits::BOARD_DIMENSION - 1,
    GomokuTraits::BOARD_DIMENSION - 1
};

const std::array<go::Position, 4> MatchTraits::DELTA = {
    go::Position{0, 1},
    go::Position{1, 0},
    go::Position{1, 1},
    go::Position{1, -1}
};
