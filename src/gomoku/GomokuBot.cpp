/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/GomokuBot.hpp"
#include "gomoku/GomokuCommand.hpp"

RatingFunction<GomokuState> GomokuBot::heuristicFn = [](const GomokuState&) {
    return 0.0;
};
RatingFunction<GomokuState> GomokuBot::utilityFn = [](const GomokuState&) {
    return 0.0;
};

GomokuBot::GomokuBot()
 : decisionTree(heuristicFn, utilityFn) {

}

std::unique_ptr<Command<GomokuState>> GomokuBot::play(const GomokuState&) {}
