/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_BOT_HPP
#define GOMOKU_BOT_HPP

#include "base/Command.hpp"
#include "MiniMaxTree.hpp"
#include "State.hpp"
#include <memory>

class GomokuBot {
 public:
    GomokuBot();
    std::unique_ptr<Command<gomoku::State>> play(const gomoku::State&);

 private:
    MiniMaxTree<gomoku::State> decisionTree;
    static RatingFunction<gomoku::State> heuristicFn;
    static RatingFunction<gomoku::State> utilityFn;
};

#endif /* GOMOKU_BOT_HPP */
