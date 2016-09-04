/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "gomoku/CommonTypes.hpp"
#include "Traits.hpp"

template<typename T>
struct Traits;

class Gomoku;
class Match;

template<>
struct Traits<Gomoku> {
    constexpr static unsigned BOARD_DIMENSION = 15;
    const static sf::Color WHITE_COLOR;
    const static sf::Color BLACK_COLOR;
    const static sf::Color WHITE_OUTLINE_COLOR;
    const static sf::Color BLACK_OUTLINE_COLOR;
    const static go::Position INVALID_POSITION;
};

template<>
struct Traits<Match> {
    constexpr static float SQUARE_SIZE = 48;
    constexpr static float BORDER_WIDTH = SQUARE_SIZE;
    constexpr static float STONE_RADIUS = 18;
    constexpr static float STONE_BORDER_WIDTH = 2;
    const static gm::Pixel INF_BOARD_LIMIAR;
    const static gm::Pixel SUP_BOARD_LIMIAR;

};

using GomokuTraits = Traits<Gomoku>;
using MatchTraits = Traits<Match>;

#endif /* TRAITS_HPP */
