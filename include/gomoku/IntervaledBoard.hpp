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

using IvMap = IntervalMap<ushort>;
using Split = std::pair<ushort, bool>;

struct Sequence {
    go::Team team;
    short origin;
    ushort size;
    IvMap holes;
    std::pair<bool, bool> openings;
};

inline std::ostream& operator<<(std::ostream& out, const Sequence& seq) {
    auto team = seq.team == go::Team::BLACK ? 0 : 1;
    out << "{" << team << "," << seq.origin << "," << seq.size << "}";
    return out;
}

class IntervaledBoard {
 public:
    IntervaledBoard();

    void play(const go::Position&, const go::Team&);

 private:
    std::list<go::Stone> stones;
    ushort currentSequence;
    std::unordered_map<ushort, Sequence> sequences;
    std::unordered_map<ushort, IvMap> rows;
    std::unordered_map<ushort, IvMap> columns;
    std::unordered_map<ushort, IvMap> mainDiagonals;
    std::unordered_map<ushort, IvMap> secondaryDiagonals;

    void solve(IvMap&, Interval, const go::Team&);
    ushort newSequence(IvMap&, Interval&, const go::Team&);
    std::pair<ushort,bool> splitSequence(const IvMap::iterator&, Interval&);
};

#endif /* INTERVALED_BOARD_HPP */
