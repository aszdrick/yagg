/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include <random>
#include <thread>
#include "AI/AIHandler.hpp"
#include "gomoku/Command.hpp"
#include "gomoku/Traits.hpp"

RatingFunction<go::State> Gomoku::AIHandler::heuristicFn = [](const go::State&) {
    return 0.0;
};
RatingFunction<go::State> Gomoku::AIHandler::utilityFn = [](const go::State&) {
    return 0.0;
};

Gomoku::AIHandler::AIHandler()
 : decisionTree(heuristicFn, utilityFn) {

}

Gomoku::AIHandler::Product Gomoku::AIHandler::doUpdate(Agent& board, Element& events) {
    using random = std::uniform_int_distribution<int>;
    static auto now = std::chrono::system_clock::now().time_since_epoch();
    static std::default_random_engine generator(now.count());
    static random pos(0, GomokuTraits::BOARD_DIMENSION - 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    base::Command<go::State> move;
    go::Position position;
    do {
        position = {pos(generator), pos(generator)};
        move.setPosition(position);
    } while (board.isOccupied(position));

    return move;
}
