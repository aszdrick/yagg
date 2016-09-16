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

struct Sequence {
    go::Team team;
    unsigned short totalSize;
    unsigned short capacity;
    bool sequential;
    std::pair<bool, bool> openings;
    std::set<unsigned short> placedPositions;

    bool updateSequentiality();
};

enum class Operation : unsigned short {
    NEW = 0,
    ADD = 1,
    MERGE = 2,
    SPLIT = 3,
    RESIZE = 4,
    ADD_RESIZE = 5
};

using IvMap = std::map<Interval, unsigned short>;
using assoc = std::pair<Interval, unsigned short>;
using Split = std::pair<assoc, assoc>;
using IntervalPair = std::pair<Interval, Interval>;
using SequenceMap = std::unordered_map<unsigned short, Sequence>;
using IntervalMap = std::unordered_map<unsigned short, IvMap>;
using IndexMapper = std::function<short(unsigned short, unsigned short)>;
using IntervalChooser = std::function<Interval(const IntervalPair&)>;

class RangeBoard {
 public:
    bool finished() const { return ended; }
    bool occupied(const go::Position& position) const;

    void play(const go::Position&, const go::Team&);
    void undo(const go::Position&);

 private:
    static const std::array<IndexMapper, 4> mappers;
    static const std::array<IntervalChooser, 4> choosers;

    std::list<go::Stone> stones;
    unsigned short currentSequence = 0;
    std::array<IntervalMap, 4> lines;
    SequenceMap sequences;
    bool ended = false;

    void undo(IvMap& map, Interval iv);
    void solve(IvMap&, Interval, const go::Team&);
    void mergeSequence(IvMap&, assoc&, Interval&);
    void mergeSequence(IvMap&, std::array<assoc, 2>&, Interval&);
    Split splitSequence(const IvMap::iterator&, Interval&);
    bool resize(IvMap&, const IvMap::iterator&, Interval&);
    unsigned short newSequence(IvMap&, Interval&, const go::Team&);
    IntervalPair generateIntervals(unsigned short,
        unsigned short, unsigned short) const;
};

#endif /* RANGE_BOARD_HPP */
