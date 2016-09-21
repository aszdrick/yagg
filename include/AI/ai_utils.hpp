/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */
#ifndef AI_UTILS_HPP
#define AI_UTILS_HPP

#include <cassert>
#include <unordered_map>
#include "extra/macros.hpp"
#include "gomoku/State.hpp"

namespace ai_utils {
    // Stores some useful properties about the board.
    // The order of the 'groups' property is:
    // (syntax: [sequence size] [number of free ends])
    // 2 1 1, 2 2 1, 3 1 1, 3 2 1, 4 1 1, 4 2 1, 5 1 1, 5 2 1
    struct BoardProperties {
        BoardProperties() : groups() {}
        std::array<unsigned, 8> groups;

        auto& score(unsigned seqSize, unsigned freeEnds) {
            constexpr unsigned minSeqSize = 2;
            constexpr unsigned maxSeqSize = 5;
            assert(seqSize >= minSeqSize);
            assert(seqSize <= maxSeqSize);
            constexpr static auto minFreeEnds = 1;
            constexpr static auto maxFreeEnds = 2;
            assert(freeEnds >= minFreeEnds);
            assert(freeEnds <= maxFreeEnds);

            const auto sizeOffset = (maxFreeEnds - minFreeEnds + 1) * (seqSize - 2);
            const auto freeEndOffset = freeEnds - minFreeEnds;
            return groups[sizeOffset + freeEndOffset];
        }
    };

    using TeamValues = std::unordered_map<go::Team, BoardProperties>;

    inline auto properties(const go::State& state) {
        TeamValues props;
        state.sequenceIteration([&](const auto& sequence) {
            auto size = sequence.stones.size();
            auto freeEnds = sequence.freeEnds.first + sequence.freeEnds.second;
            auto team = (*sequence.stones.front()).team;
            ++props[team].score(size, freeEnds);                
        });
        return props;
    }

    // inline auto properties(const go::State& state) {
    //     TeamValues props;
    //     const auto& classifiers = state.retrieveClassifiers();

    //     for (auto i = 0; i < 2; i++) {
    //         auto team = static_cast<go::Team>(i);
    //         props[team].groups[0] = classifiers.at(i).at(2).at(0).at(1).size();
    //         props[team].groups[0] += classifiers.at(i).at(2).at(1).at(1).size();
    //         props[team].groups[1] = classifiers.at(i).at(2).at(0).at(2).size();
    //         props[team].groups[1] += classifiers.at(i).at(2).at(1).at(2).size();

    //         props[team].groups[2] = classifiers.at(i).at(3).at(0).at(1).size();
    //         props[team].groups[2] += classifiers.at(i).at(3).at(1).at(1).size();
    //         props[team].groups[3] = classifiers.at(i).at(3).at(0).at(2).size();
    //         props[team].groups[3] += classifiers.at(i).at(3).at(1).at(2).size();

    //         props[team].groups[4] = classifiers.at(i).at(4).at(0).at(1).size();
    //         props[team].groups[4] += classifiers.at(i).at(4).at(1).at(1).size();
    //         props[team].groups[5] = classifiers.at(i).at(4).at(0).at(2).size();
    //         props[team].groups[5] += classifiers.at(i).at(4).at(1).at(2).size();

    //         props[team].groups[6] = classifiers.at(i).at(5).at(0).at(1).size();
    //         props[team].groups[6] += classifiers.at(i).at(5).at(1).at(1).size();
    //         props[team].groups[7] = classifiers.at(i).at(5).at(0).at(2).size();
    //         props[team].groups[7] += classifiers.at(i).at(5).at(1).at(2).size();
    //     }
    //     return props;
    // }

    template<typename Iterable>
    inline auto applyWeights(const TeamValues& props,
        const Iterable& weightList, short player, double w1 = 1, double w2 = 1) {

        std::unordered_map<go::Team, double> sumPerTeam;
        for (auto& pair : props) {
            double result = 0;
            size_t i = 0;
            for (auto& wt : weightList) {
                result += pair.second.groups[i] * wt;
                i++;
            }
            sumPerTeam[pair.first] = result;
        }
        auto self = sumPerTeam[go::Team::BLACK];
        auto other = sumPerTeam[go::Team::WHITE];
        if (player == 1) {
            self += other;
            other = self - other;
            self -= other;
        }
        return self * w1 - other * w2;
    }

    template<typename Iterable, typename... Args>
    inline auto applyWeights(const go::State& state,
        const Iterable& weightList, short player, Args&&... args) {
        
        return applyWeights(properties(state),
            weightList, player, std::forward<Args>(args)...);
    }

    template<unsigned id>
    inline auto heuristic(const go::State&, unsigned, short);

    template<unsigned id>
    inline auto utility(const go::State& state, unsigned level, short player);

    // ------------------------ AI ------------------------ //

    template<>
    inline auto heuristic<1>(const go::State& state, unsigned level, short player) {
        constexpr auto weights = {
            1, 3, 3, 7, 1, 1000, 0, 0
        };
        return applyWeights(state, weights, player, 1.5, 1) * (1 + 1.0/level);
    };

    template<>
    inline auto utility<1>(const go::State& state, unsigned level, short player) {
        constexpr auto weights = {
            1, 3, 3, 7, 1, 1000, 9999, 10000
        };
        return applyWeights(state, weights, player, 1.5, 1) * (1 + 1.0/level);
    }
}

#endif /* AI_UTILS_HPP */
