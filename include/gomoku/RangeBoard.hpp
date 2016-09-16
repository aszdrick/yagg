/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef RANGE_BOARD_HPP
#define RANGE_BOARD_HPP

#include <array>
#include <ostream>
#include <set>
#include <unordered_map>
#include "gomoku/CommonTypes.hpp"
#include "extra/Matrix.hpp"
#include "extra/Interval.hpp"

using IvMap = std::map<Interval, unsigned short>;
using assoc = std::pair<Interval, unsigned short>;
using Split = std::pair<assoc, assoc>;

struct Sequence {
    go::Team team;
    unsigned short totalSize;
    unsigned short capacity;
    bool sequential;
    std::pair<bool, bool> openings;
    std::set<unsigned short> placedPositions;

    bool updateSequentiality();
};

class RangeBoard {
 public:
    RangeBoard();

    bool finished() const { return ended; }
    bool occupied(const go::Position& position) const;

    void play(const go::Position&, const go::Team&);

 private:
    std::list<go::Stone> stones;
    unsigned short currentSequence;
    std::unordered_map<unsigned short, Sequence> sequences;
    std::unordered_map<unsigned short, IvMap> rows;
    std::unordered_map<unsigned short, IvMap> columns;
    std::unordered_map<unsigned short, IvMap> mainDiagonals;
    std::unordered_map<unsigned short, IvMap> secondaryDiagonals;
    bool ended = false;

    void solve(IvMap&, Interval, const go::Team&);
    void mergeSequence(IvMap&, assoc&, Interval&);
    void mergeSequence(IvMap&, std::array<assoc, 2>&, Interval&);
    Split splitSequence(const IvMap::iterator&, Interval&);
    bool resize(IvMap&, const IvMap::iterator&, Interval&); 
    unsigned short newSequence(IvMap&, Interval&, const go::Team&);
};

#endif /* RANGE_BOARD_HPP */
