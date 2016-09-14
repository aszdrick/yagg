/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <chrono>
#include <random>
#include <thread>
#include "AI/ai_utils.hpp"
// #include "AI/AIHandler.hpp"
#include "extra/macros.hpp"
#include "gomoku/Command.hpp"
#include "gomoku/Traits.hpp"

template<unsigned id>
RatingFunction<go::State> Gomoku::AIHandler<id>::heuristicFn = ai_utils::heuristic<id>;
template<unsigned id>
RatingFunction<go::State> Gomoku::AIHandler<id>::utilityFn = ai_utils::utility<id>;

template<unsigned id>
Gomoku::AIHandler<id>::AIHandler()
 : decisionTree(heuristicFn, utilityFn) {

}

template<unsigned id>
typename Gomoku::AIHandler<id>::Product
Gomoku::AIHandler<id>::doUpdate(Agent& board, Element& events) {
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
    // auto analysis = decisionTree.analyze(board);
    // analysis.move.setIterations(analysis.depth);
    // return analysis.move;
}
