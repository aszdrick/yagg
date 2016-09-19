/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/SearchSpace.hpp"
#include "gomoku/Traits.hpp"

SearchSpace::SearchSpace() {
    init();
}

void SearchSpace::play(const go::Position& position) {
    constexpr static auto tolerance = 2;
    constexpr static auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    bool inserted;
    for (int i = -tolerance; i <= tolerance; i++) {
        if (i < 0 || i >= boardDimension) continue;
        for (int j = -tolerance; j <= tolerance; j++) {
            if (j < 0 || j >= boardDimension) continue;
            if ((i | j) == 0) continue;
            std::tie(std::ignore, inserted) = space.insert(position + go::Position{i, j});
            magicEntity.push(inserted);
        }
    }
    // history.push_back(position);
    magicEntity.push(space.erase(position));
    BLANK
    ECHO("PLAY");
    TRACE_IT(space);
    // ECHO("[INSERT] " + std::to_string(space.size()));
    // space.erase(position);
}

void SearchSpace::undo(const go::Position& position) {
    constexpr static auto tolerance = 2;
    constexpr static auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    if (magicEntity.top()) {
        space.insert(position);
    }
    magicEntity.pop();
    for (int i = tolerance; i >= -tolerance; i--) {
        if (i < 0 || i >= boardDimension) continue;
        for (int j = tolerance; j >= -tolerance; j--) {
            if (j < 0 || j >= boardDimension) continue;
            if ((i | j) == 0) continue;
            auto inserted = magicEntity.top();
            if (inserted) {
                space.erase(position + go::Position{i, j});
            }
            magicEntity.pop();
        }
    }
    BLANK
    ECHO("UNDO");
    TRACE_IT(space);
    // for (int i = -tolerance; i <= tolerance; i++) {
    //     if (i < 0 || i >= boardDimension) continue;
    //     for (int j = -tolerance; j <= tolerance; j++) {
    //         if (j < 0 || j >= boardDimension) continue;
    //         std::tie(std::ignore, inserted) = space.insert(position + go::Position{i, j});
    //         magicEntity.push(inserted);
    //     }
    // }

    // space.clear();
    // decltype(history) copy;
    // history.swap(copy);

    // copy.pop_back();
    // while (!copy.empty()) {
    //     play(copy.front());
    //     copy.pop_front();
    // }

    // if (space.empty()) {
    //     init();
    // }
    // ECHO("[UNDO] " + std::to_string(space.size()));
    // space.insert(position);
}

void SearchSpace::init() {
    std::pair<int, int> centerBounds{6, 8};
    for (int i = centerBounds.first; i <= centerBounds.second; i++) {
        for (int j = centerBounds.first; j <= centerBounds.second; j++) {
            space.insert(go::Position{i, j});
        }
    }
    // for (int i = 0; i < 15; i++) {
    //     for (int j = 0; j < 15; j++) {
    //         space.insert(go::Position{i,j});
    //     }
    // }
}
