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

    void iterate(const std::function<void(const Stone&)>& fn) const {
        analyzer.iterate(fn);
    }

    void quadrupletIteration(const BoardAnalyzer::SequenceCallback& fn) const {
        analyzer.quadrupletIteration(fn);
    }

    void sequenceIteration(const BoardAnalyzer::SequenceCallback& fn) const {
        analyzer.sequenceIteration(fn);
    }

    auto currentPlayer() const {
        return player;
    }
    
    auto winnerPlayer() const {
        return winner;
    }
    
    auto countEmptySquares() const {
        return analyzer.countEmptySquares();
    }

    bool occupied(const go::Position& pos) const {
        return analyzer.occupied(pos);
        // return ivBoard.occupied(pos);
    }

    bool over() const { 
        // return analyzer.over();
        return ivBoard.finished();
    }

    bool hasWinner() const {
        // return analyzer.hasWinner();
        return ivBoard.finished();
    }

    bool full() const {
        return analyzer.full();
    }

    void undo() {
        analyzer.undo();
        player = 1 - player;
    }

 private:
    short player = 0;
    decltype(player) winner = 0;
    BoardAnalyzer analyzer;
    IvBoard ivBoard;
};

#endif /* GO_STATE_HPP */
