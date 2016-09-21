/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef AI_HANDLER_HPP
#define AI_HANDLER_HPP

#include "gomoku/Player.hpp"
#include "MiniMaxTree.hpp"
#include "gomoku/State.hpp"
#include <memory>

template<unsigned id>
class Gomoku::AIHandler : public Player::InputComponent {
 public:
    AIHandler();

 private:
    static RatingFunction<go::State, short> heuristicFn;
    static RatingFunction<go::State, short> utilityFn;
    decltype(make_minimax(heuristicFn, utilityFn)) decisionTree;

    Product doUpdate(Agent&, Element&) override;
};

#include "AIHandler.ipp"

#endif /* AI_HANDLER_HPP */
