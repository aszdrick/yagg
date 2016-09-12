/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GO_STATE_HPP
#define GO_STATE_HPP

#include "BoardAnalyzer.hpp"
#include "CommonTypes.hpp"
#include <functional>
#include <unordered_map>
#include "Zoas.hpp"

class go::State {
 public:
    void play(const go::Position&, go::Team);
    void iterate(const std::function<void(const Stone&)>&) const;
    short currentPlayer() const;
    short winnerPlayer() const;
    bool isOver() const;
    void quadrupletIteration(const BoardAnalyzer::SequenceCallback&) const;

 private:
    short player = 0;
    short winner = 0;
    BoardAnalyzer analyzer;
    Zoas hue;
};

#endif /* GO_STATE_HPP */
