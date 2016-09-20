/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef RANGE_BOARD_HPP
#define RANGE_BOARD_HPP

#include <array>
#include <cmath>
#include <ostream>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "CommonTypes.hpp"
#include "extra/Interval.hpp"
#include "SearchSpace.hpp"

struct Sequence {
    go::Team team;
    unsigned short totalSize;
    unsigned short capacity;
    unsigned short fragmentation;
    std::pair<bool, bool> openings;
    std::set<unsigned short> positions;

    bool updateSequentiality();
    bool checkWinCondition() const;
    std::list<unsigned short> findCriticalHoles() const;
};

class RangeBoard {
 public:
    using StoneCallback = std::function<void(const go::Stone&)>;
    using PositionCallback = std::function<void(const go::Position&)>;
    using Undo = std::function<void(void)>;
    using IvMap = std::map<Interval, unsigned>;
    using assoc = std::pair<Interval, unsigned short>;
    using SequenceMap = std::unordered_map<unsigned short, Sequence>;
    using IntervalMap = std::unordered_map<unsigned short, IvMap>;
    using IndexMapper = std::function<short(unsigned short, unsigned short)>;
    using IndexUnmapper = std::function<go::Position(unsigned short, unsigned short)>;
    using RangeChooser = std::function<Interval(unsigned short, unsigned short)>;
    using ClassifierMap = std::array<std::array<std::array<std::unordered_set<unsigned short>,3>,8>,15>;

    bool tie() const;
    bool finished() const;
    bool occupied(const go::Position& position) const;
    unsigned countEmptySquares() const;
    const auto& possibleSquares() const;

    const std::array<ClassifierMap, 2> retrieveClassifiers() const {
        return classified_sequences;
    }

    void iterate(const StoneCallback&) const;
    void iterateCriticalZone(const PositionCallback&) const;
    void play(const go::Position&, go::Team);
    void undo();

 private:
    static const std::array<IndexMapper, 4> mappers;
    static const std::array<IndexUnmapper, 4> unmappers;
    static const std::array<RangeChooser, 4> choosers;

    std::list<go::Stone> stones;
    std::set<go::Position> placed_positions;
    std::array<IntervalMap, 4> lines;
    std::array<unsigned, 2> dominations;
    std::array<ClassifierMap, 2> classified_sequences;
    SequenceMap sequences;
    SearchSpace search_space;
    unsigned currentSequence = 0;
    unsigned sequencesCount = 0;
    unsigned deadSequences = 0;
    bool ended = false;
    std::stack<unsigned short> merge_keys;
    std::stack<bool> ended_values;

    void classify(unsigned short, const Sequence&, const Interval&);
    void unclassify(unsigned short, const Sequence&, const Interval&);

    void solve(IvMap&, Interval, go::Team);
    void create(IvMap&, Interval&, go::Team);
    void increase(IvMap&, assoc&, Interval&);
    void merge(IvMap&, std::array<assoc, 2>&, Interval&);
    void split(IvMap& map, const IvMap::iterator&, Interval&);
    bool resize(IvMap&, const IvMap::iterator&, Interval&);
    assoc premerge(IvMap&, const IvMap::iterator&);

    void undoCreate(IvMap&, const IvMap::iterator&);
    void undoCentralMove(IvMap&, const IvMap::iterator&, const Interval&);
    void undoMerge(IvMap&, const IvMap::iterator&, const Interval&);
    void undoSideMove(IvMap&, const IvMap::iterator&, const Interval&,
        const Interval&, const Interval&);
    Interval undoIncrease(IvMap&, const IvMap::iterator&, const Interval&);
    void undoSplit(IvMap&, const IvMap::iterator&);
    void undoSplit(IvMap&, const IvMap::iterator&, const IvMap::iterator&);
    void undoResize(IvMap&, const IvMap::iterator&, const Interval&,
        const Interval&);
    void undoResize(IvMap&, const IvMap::iterator&, const Interval&,
        unsigned short);
};

inline bool RangeBoard::finished() const {
    return ended || tie();
}

inline bool RangeBoard::tie() const {
    return sequencesCount != 0 && sequencesCount == deadSequences;
}

inline bool RangeBoard::occupied(const go::Position& position) const {
    return placed_positions.count(position);
}

inline unsigned RangeBoard::countEmptySquares() const {
    static constexpr auto maxStones = std::pow(GomokuTraits::BOARD_DIMENSION, 2);
    return maxStones - stones.size();    
}

inline const auto& RangeBoard::possibleSquares() const {
    return search_space.squares();
}

#endif /* RANGE_BOARD_HPP */
