/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include <random>
#include <thread>
#include "AI/RandomHandler.hpp"
#include "extra/macros.hpp"
#include "gomoku/Command.hpp"
#include "gomoku/Traits.hpp"
#include "gomoku/State.hpp"

RandomHandler::RandomHandler() { }

typename RandomHandler::Product
RandomHandler::doUpdate(Agent& board, Element& events) {
    using random = std::uniform_int_distribution<int>;
    static auto now = std::chrono::system_clock::now().time_since_epoch();
    static std::default_random_engine generator(now.count());
    static random pos(0, GomokuTraits::BOARD_DIMENSION - 1);
    static random r_delta(-1, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    base::Command<go::State> move;
    go::Position position;
    position = {pos(generator), pos(generator)};
    while (board.occupied(position)) {
        position = position + go::Position{r_delta(generator), r_delta(generator)};
    }
    move.setPosition(position);
    move.setIterations(pos(generator) + 1);
    return move;
}
