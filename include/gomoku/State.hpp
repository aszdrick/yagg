/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GO_STATE_HPP
#define GO_STATE_HPP

#include "BoardAnalyzer.hpp"
#include "CommonTypes.hpp"
#include "IntervaledBoard.hpp"
#include <functional>
#include <unordered_map>

class go::State {
 public:
    void play(const go::Position&, go::Team);
    void iterate(const std::function<void(const Stone&)>&) const;
    short currentPlayer() const;
    short winnerPlayer() const;
    void quadrupletIteration(const BoardAnalyzer::SequenceCallback&) const;
    bool occupied(const go::Position&) const;
    bool over() const;
    bool hasWinner() const;
    bool full() const;
    State generateNext() const {
        return go::State();
    }

 private:
    short player = 0;
    short winner = 0;
    BoardAnalyzer analyzer;
    IntervaledBoard ivBoard;
};

#endif /* GO_STATE_HPP */
