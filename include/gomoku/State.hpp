/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GO_STATE_HPP
#define GO_STATE_HPP

#include "CommonTypes.hpp"
#include <functional>
#include <unordered_map>

class go::State {
 public:
    void play(const go::Position&, go::Team);
    void iterate(const std::function<void(const Stone&)>&) const;
    short currentPlayer() const;

 private:
    short player = 0;
    using Board = std::unordered_map<unsigned,
                    std::unordered_map<unsigned, Stone>>;
    Board board;

    bool isOccupied(const go::Position&) const;
};

#endif /* GO_STATE_HPP */
