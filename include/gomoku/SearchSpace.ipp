/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Traits.hpp"

namespace {
    template<bool B>
    struct advance_t;

    template<>
    struct advance_t<false> {
        static void advance(int& value) {
            --value;
        }
    };

    template<>
    struct advance_t<true> {
        static void advance(int& value) {
            ++value;
        }
    };

    template<bool B>
    constexpr auto advance = advance_t<B>::advance;

    template<bool B>
    struct condition_t;

    template<>
    struct condition_t<false> {
        using check = std::greater_equal<int>;
    };

    template<>
    struct condition_t<true> {
        using check = std::less_equal<int>;
    };

    template<bool B>
    constexpr auto condition = typename condition_t<B>::check();

    template<int start, int end>
    void traverse(const go::Position& pos,
        const std::function<void(const go::Position&)>& fn) {

        constexpr static int boardDimension = GomokuTraits::BOARD_DIMENSION;
        constexpr static auto lt = start < end;
        for (int i = start; condition<lt>(i, end); advance<lt>(i)) {
            auto row = pos.row + i;
            if (row < 0 || row >= boardDimension) continue;
            for (int j = start; condition<lt>(j, end); advance<lt>(j)) {
                if ((i | j) == 0) continue;
                auto column = pos.column + j;
                if (column < 0 || column >= boardDimension) continue;
                fn(go::Position{row, column});
            }
        }
    }
}

template<typename T>
void SearchSpace::play(const go::Position& position, const T& verifier) {
    constexpr static auto tolerance = 2;
    traverse<-tolerance, tolerance>(position, [=](const auto& pos) {
        bool inserted = false;
        if (!verifier.occupied(pos)) {
            std::tie(std::ignore, inserted) = space.insert(pos);
        }
        keeper.push(inserted);
    });
    keeper.push(space.erase(position));
}

template<typename T>
void SearchSpace::undo(const go::Position& position, const T& verifier) {
    constexpr static auto tolerance = 2;
    if (keeper.top()) {
        space.insert(position);
    }
    keeper.pop();
    traverse<tolerance, -tolerance>(position, [=](const auto& position) {
        auto inserted = keeper.top();
        if (inserted) {
            space.erase(position);
        }
        keeper.pop();
    });
}

inline void SearchSpace::init() {
    std::pair<int, int> centerBounds{6, 8};
    for (int i = centerBounds.first; i <= centerBounds.second; i++) {
        for (int j = centerBounds.first; j <= centerBounds.second; j++) {
            space.insert(go::Position{i, j});
        }
    }
}
