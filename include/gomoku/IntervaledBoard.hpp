/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef INTERVALED_BOARD_HPP
#define INTERVALED_BOARD_HPP

#include <array>
#include <unordered_map>
#include <ostream>
#include "gomoku/CommonTypes.hpp"
#include "extra/Matrix.hpp"
#include "extra/Interval.hpp"

using IvMap = IntervalMap<unsigned short>;
using Split = std::pair<unsigned short, bool>;

struct Sequence {
    go::Team team;
    short origin;
    unsigned short localSize;
    unsigned short totalSize;
    unsigned short capacity;
    IvMap holes;
};

inline std::ostream& operator<<(std::ostream& out, const Sequence& seq) {
    // auto team = seq.team == go::Team::BLACK ? 0 : 1;
    // out << "{" << team << "," << seq.origin << "," << seq.size << "}";
    return out;
}

class IntervaledBoard {
 public:
    IntervaledBoard();

    void play(const go::Position&, const go::Team&);

 private:
    std::list<go::Stone> stones;
    unsigned short currentSequence;
    std::unordered_map<unsigned short, Sequence> sequences;
    std::unordered_map<unsigned short, IvMap> rows;
    std::unordered_map<unsigned short, IvMap> columns;
    std::unordered_map<unsigned short, IvMap> mainDiagonals;
    std::unordered_map<unsigned short, IvMap> secondaryDiagonals;

    void solve(IvMap&, Interval, const go::Team&);
    void mergeSequence(IvMap&, IvMap::assoc&, Interval&);
    Split splitSequence(const IvMap::iterator&, Interval&);
    unsigned short newSequence(IvMap&, Interval&, const go::Team&);
    void generateHole(Sequence&, Interval&, Interval&);
    void fillHole(Sequence&, Interval&, Interval&);
    IvMap::iterator findHole(Sequence&, Interval);
};

#endif /* INTERVALED_BOARD_HPP */
