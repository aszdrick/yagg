/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */
#ifndef AI_UTILS_HPP
#define AI_UTILS_HPP

#include "gomoku/State.hpp"

namespace ai_utils {
    template<unsigned id>
    inline constexpr auto heuristic(const go::State&);

    template<unsigned id>
    inline constexpr auto utility(const go::State&);

    template<>
    constexpr auto heuristic<1>(const go::State&) {
        return 1.0;
    };

    template<>
    constexpr auto utility<1>(const go::State&) {
        return 2.0;
    };


    template<>
    constexpr auto heuristic<2>(const go::State&) {
        return 3.0;
    };

    template<>
    constexpr auto utility<2>(const go::State&) {
        return 4.0;
    };
}

#endif /* AI_UTILS_HPP */
