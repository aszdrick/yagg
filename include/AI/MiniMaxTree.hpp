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

template<typename T>
using RatingFunction = std::function<double(const T&)>;

template<typename T>
class MiniMaxTree {
    struct Node;
    struct AnalysisReport;
 public:
    MiniMaxTree(const RatingFunction<T>&, const RatingFunction<T>&);
    template<typename Generator>
    AnalysisReport analyze(const T&);

 private:
    Node root;
    const RatingFunction<T>& h_function;
    const RatingFunction<T>& u_function;

    template<typename Generator>
    Node& update(Node&, unsigned);
};

template<typename T>
struct MiniMaxTree<T>::Node {
    T state;
    Type type;
    double value = 0;
    double alpha = -INT_MAX;
    double beta = INT_MAX;
    std::vector<Node> children;
};

template<typename T>
struct MiniMaxTree<T>::AnalysisReport {
    base::Command<T> move;
    unsigned depth;
};

namespace std {
    template<>
    struct hash<Type> {
        std::size_t operator()(const Type& type) const {
            return static_cast<int>(type);
        }
    };
}

template<typename T>
struct initial_values {
    double best;
    double* param;
    std::function<T(T,T)> updater;
    Type nextType;
};

#include "MiniMaxTree.ipp"

#endif /* MINI_MAX_TREE_HPP */
