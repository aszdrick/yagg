/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef RANGE_BOARD_HPP
#define RANGE_BOARD_HPP

#include <array>
#include <ostream>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
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

using Undo = std::function<void(void)>;
using IvMap = std::map<Interval, unsigned short>;
using assoc = std::pair<Interval, unsigned short>;
using SequenceMap = std::unordered_map<unsigned short, Sequence>;
using IntervalMap = std::unordered_map<unsigned short, IvMap>;
using IndexMapper = std::function<short(unsigned short, unsigned short)>;
using RangeChooser = std::function<Interval(unsigned short, unsigned short)>;
using ClassifierMap = std::array<std::unordered_set<unsigned short>, 8>;

class RangeBoard {
 public:
    bool finished() const { return ended; }
    bool occupied(const go::Position& position) const;

    void play(const go::Position&, go::Team);
    void undo();

 private:
    static const std::array<IndexMapper, 4> mappers;
    static const std::array<RangeChooser, 4> choosers;

    std::list<go::Stone> stones;

    unsigned short currentSequence = 0;
    std::array<IntervalMap, 4> lines;
    std::array<unsigned, 2> dominations;
    std::array<ClassifierMap, 2> classified_sequences;
    SequenceMap sequences;
    bool ended = false;

    void solve(IvMap&, Interval, go::Team);
    
    void create(IvMap&, Interval&, go::Team);
    void increase(IvMap&, assoc&, Interval&, go::Team);
    void merge(IvMap&, std::array<assoc, 2>&, Interval&, go::Team);
    void split(IvMap& map, const IvMap::iterator&, Interval&);
    bool resize(IvMap&, const IvMap::iterator&, Interval&);
    assoc premerge(IvMap&, const IvMap::iterator&);

    void undoCreate(IvMap&, const IvMap::iterator&);
    void undoCentralMove(IvMap&, const IvMap::iterator&, const Interval&);
    void undoMerge(IvMap&, const IvMap::iterator&);
    void undoIncrease(IvMap&, const IvMap::iterator&, const Interval&);
    void undoSplit(IvMap&, const IvMap::iterator&);
    void undoSplit(IvMap&, const IvMap::iterator&, const IvMap::iterator&);
    void undoResize(IvMap&, const IvMap::iterator&,
        const Interval&, const Interval&);
    void undoResize(IvMap&, const IvMap::iterator&,
        const Interval&, unsigned short);
};

#endif /* RANGE_BOARD_HPP */
