/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef MINI_MAX_TREE_HPP
#define MINI_MAX_TREE_HPP

#include "base/Command.hpp"
#include <climits>
#include <functional>
#include <memory>
#include <vector>

enum class Type {
    MIN, MAX
};

namespace minimax {
    template<typename T>
    struct AnalysisReport {
        base::Command<T> move;
        unsigned iterations;
    };    

    template<typename T>
    struct initial_values {
        double best;
        double* param;
        std::function<T(T,T)> updater;
        Type nextType;
    };
}

template<typename T, typename... Args>
using RatingFunction = std::function<double(const T&, unsigned, Args...)>;

template<typename T, typename... Args>
class MiniMaxTree {
 public:
    MiniMaxTree(const RatingFunction<T, Args...>&,
        const RatingFunction<T, Args...>&);

    template<typename Generator>
    minimax::AnalysisReport<T> analyze(T&, Args...);

 private:
    const RatingFunction<T, Args...>& h_function;
    const RatingFunction<T, Args...>& u_function;
    unsigned maxDepth;

    template<typename Generator>
    double calculate(Generator&, const T&, unsigned, Args...) const;

    template<typename Generator>
    double exec(Generator&, const T&, unsigned, double, double, Type, Args...) const;
};

template<typename T, typename... Args>
MiniMaxTree<T, Args...> make_minimax(const RatingFunction<T, Args...>& h,
    const RatingFunction<T, Args...>& u) {

    return MiniMaxTree<T, Args...>(h, u);
}

namespace std {
    template<>
    struct hash<Type> {
        std::size_t operator()(const Type& type) const {
            return static_cast<int>(type);
        }
    };
}

#include "MiniMaxTree.ipp"

#endif /* MINI_MAX_TREE_HPP */
