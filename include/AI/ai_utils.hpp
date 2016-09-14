/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */
#ifndef AI_UTILS_HPP
#define AI_UTILS_HPP

#include <cassert>
#include "gomoku/State.hpp"

namespace ai_utils {
    // Stores some useful properties about the board.
    // The order of the 'groups' property is:
    // (syntax: [sequence size] [number of free ends] [number of sequences])
    // 2 1 1, 2 2 1, 3 1 1, 3 2 1, 4 1 1, 4 2 1, 5 1 1, 5 2 1,
    // 2 2 2, 2 3 2, 2 4 2, 3 2 2, 3 3 2, 3 4 2, 4 2 2, 4 3 2, 4 4 2
    struct BoardProperties {
        std::array<unsigned, 17> groups;
        auto& score(unsigned seqSize, unsigned freeEnds, unsigned numSeqs = 1) {
            constexpr unsigned minSeqSize = 2;
            constexpr unsigned maxSeqSize = 5;
            assert(seqSize >= minSeqSize);
            assert(seqSize <= maxSeqSize);
            assert(numSeqs >= 1);
            assert(numSeqs <= 2);
            const auto minFreeEnds = numSeqs;
            const auto maxFreeEnds = 2 * numSeqs;
            assert(freeEnds >= minFreeEnds);
            assert(freeEnds <= maxFreeEnds);

            constexpr unsigned numSimpleSeqs = 2 * (maxSeqSize - minSeqSize + 1);
            const auto baseOffset = numSimpleSeqs * (numSeqs - 1);
            const auto sizeOffset = (maxFreeEnds - minFreeEnds + 1) * (seqSize - 2);
            const auto freeEndOffset = freeEnds - minFreeEnds;
            return groups[baseOffset + sizeOffset + freeEndOffset];
        }
    };

    inline auto properties(const go::State& states) {
        // TODO
        return BoardProperties();
    }

    template<typename Iterable>
    inline auto applyWeights(const BoardProperties& props, const Iterable& weightList) {
        double result = 0;
        size_t i = 0;
        for (auto& wt : weightList) {
            result += props.groups[i] * wt;
            i++;
        }
        return result;
    }

    template<typename Iterable>
    inline auto applyWeights(const go::State& state, const Iterable& weightList) {
        return applyWeights(properties(state), weightList);
    }

    template<unsigned id>
    inline auto heuristic(const go::State&);

    template<unsigned id>
    inline auto utility(const go::State&);

    // ------------------------ First AI ------------------------ //

    template<>
    auto heuristic<1>(const go::State& state) {
        // constexpr auto weights = {
        //     10., 20., 100., 200., 1e4, 2e4, 1e8, 2e8,
        //     2000., 3000., 4000., 2e5, 3e5, 4e5, 2e6, 3e6, 4e6
        // };
        // return applyWeights(state, weights);
        return 1.0;
    };

    template<>
    auto utility<1>(const go::State&) {
        return 2.0;
    };

    // ------------------------ Second AI ------------------------ //

    template<>
    auto heuristic<2>(const go::State&) {
        return 3.0;
    };

    template<>
    auto utility<2>(const go::State&) {
        return 4.0;
    };
}

#endif /* AI_UTILS_HPP */
