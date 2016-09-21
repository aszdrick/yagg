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
    // (syntax: [sequence size] [number of free ends] [number of sequences])
    // 2 1 1, 2 2 1, 3 1 1, 3 2 1, 4 1 1, 4 2 1, 5 1 1, 5 2 1,
    // 2 2 2, 2 3 2, 2 4 2, 3 2 2, 3 3 2, 3 4 2, 4 2 2, 4 3 2, 4 4 2
    struct BoardProperties {
        BoardProperties() : groups() {}
        std::array<unsigned, 17> groups;

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
                // result += props.groups[i] * wt;
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

    // ------------------------ First AI ------------------------ //

    template<>
    inline auto heuristic<1>(const go::State& state, unsigned, short player) {
        constexpr auto weights = {
            10., 20., 100., 200., 1e4, 2e4, 1e8, 2e8,
            2000., 3000., 4000., 2e5, 3e5, 4e5, 2e6, 3e6, 4e6
        };
        return applyWeights(state, weights, player);
    };

    template<>
    inline auto utility<1>(const go::State& state, unsigned level, short player) {
        return heuristic<1>(state, level, player) * (1 + 1.0/level);
    }

    // ------------------------ Second AI ------------------------ //

    template<>
    inline auto heuristic<2>(const go::State& state, unsigned, short player) {
        constexpr auto weights = {
            // 10., 20., 50., 200., 1000., 2000., 1e6, 2e6,
            1, 3, 5, 8, 6, 100, 9999, 10000,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        return applyWeights(state, weights, player, 1.5, 1);
    };

    template<>
    inline auto utility<2>(const go::State& state, unsigned level, short player) {
        return heuristic<2>(state, level, player) * (1 + 1.0/level);
    }

    // ------------------------ Third AI ------------------------ //

    template<>
    inline auto heuristic<3>(const go::State& state, unsigned, short player) {
        constexpr auto weights = {
            1, 7, 2, 10, 3, 1000, 99999, 100000,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        return applyWeights(state, weights, player, 1.5, 1);
    };

    template<>
    inline auto utility<3>(const go::State& state, unsigned level, short player) {
        return heuristic<3>(state, level, player) * (1 + 1.0/level);
    }

    // ------------------------ Fourth AI ------------------------ //

    template<>
    inline auto heuristic<4>(const go::State& state, unsigned, short player) {
        constexpr auto weights = {
            1, 3, 3, 7, 0, 1000, 9999, 10000,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        return applyWeights(state, weights, player, 1.5, 1);
    };

    template<>
    inline auto utility<4>(const go::State& state, unsigned level, short player) {
        return heuristic<4>(state, level, player) * (1 + 1.0/level);
    }

    // ------------------------ Fifth AI ------------------------ //

    template<>
    inline auto heuristic<5>(const go::State& state, unsigned, short player) {
        constexpr auto weights = {
            0, 5, 0, 10, 0, 1000, 9999, 10000,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        return applyWeights(state, weights, player, 1.5, 1);
    };

    template<>
    inline auto utility<5>(const go::State& state, unsigned level, short player) {
        return heuristic<5>(state, level, player) * (1 + 1.0/level);
    }

    // ------------------------ Sixth AI ------------------------ //

    template<>
    inline auto heuristic<6>(const go::State& state, unsigned, short player) {
        constexpr auto weights = {
            2, 5, 4, 11, 1, 100, 9999, 10000,
            0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        return applyWeights(state, weights, player, 1.2, 1);
    };

    template<>
    inline auto utility<6>(const go::State& state, unsigned level, short player) {
        return heuristic<6>(state, level, player) * (1 + 1.0/level);
    }

    // ------------------------ Fourth AI ------------------------ //

    // template<>
    // inline auto heuristic<4>(const go::State& state, unsigned, short player) {
    //     constexpr auto weights = {
    //         1, 3, 3, 7, 0, 1000, 9999, 10000,
    //         0, 0, 0, 0, 0, 0, 0, 0, 0
    //     };
    //     return applyWeights(state, weights, player, 1.5, 1);
    // };

    // template<>
    // inline auto utility<6>(const go::State& state, unsigned level, short player) {
    //     return heuristic<6>(state, level, player) * (1 + 1.0/level);
    // }
}

#endif /* AI_UTILS_HPP */
