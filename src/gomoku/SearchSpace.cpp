/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/SearchSpace.hpp"

SearchSpace::SearchSpace() {
    init();
}

void SearchSpace::play(const go::Position& position) {
    // constexpr static auto tolerance = 2;
    // // bool inserted;
    // for (int i = -tolerance; i <= tolerance; i++) {
    //     for (int j = -tolerance; j <= tolerance; j++) {
    //         space.insert(position + go::Position{i, j});
    //         // auto pos = position + go::Position{i, j};
    //         // std::tie(std::ignore, inserted) = space.insert(pos);
    //         // if (inserted) {
    //         //     fragile.insert(pos);
    //         // } else {
    //         //     fragile.erase(pos);
    //         // }
    //     }
    // }
    // history.push_back(position);
    // space.erase(position);
    // // ECHO("[INSERT] " + std::to_string(space.size()));
    space.erase(position);
}

void SearchSpace::undo(const go::Position& position) {
    // space.erase(position);
    // space.clear();
    // init();

    // decltype(history) copy;
    // history.swap(copy);

    // copy.pop_back();
    // while (!copy.empty()) {
    //     play(copy.front());
    //     copy.pop_front();
    // }
    // ECHO("[UNDO] " + std::to_string(space.size()));
    // for (auto& pos : copy) {
    //     play(pos);
    // }
    space.insert(position);
}

void SearchSpace::init() {
    // std::pair<int, int> centerBounds{6, 8};
    // for (int i = centerBounds.first; i <= centerBounds.second; i++) {
    //     for (int j = centerBounds.first; j <= centerBounds.second; j++) {
    //         space.insert(go::Position{i, j});
    //     }
    // }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            space.insert(go::Position{i,j});
        }
    }
}
