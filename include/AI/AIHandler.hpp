/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef AI_HANDLER_HPP
#define AI_HANDLER_HPP

#include "gomoku/Player.hpp"
#include "MiniMaxTree.hpp"
#include "gomoku/State.hpp"
#include <memory>

class Gomoku::AIHandler : public Player::InputComponent {
 public:
    AIHandler();

 private:
    MiniMaxTree<go::State> decisionTree;
    static RatingFunction<go::State> heuristicFn;
    static RatingFunction<go::State> utilityFn;

    Product doUpdate(Agent&, Element&) override;
};

#endif /* AI_HANDLER_HPP */
