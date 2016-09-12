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
 public:
    MiniMaxTree(const RatingFunction<T>&, const RatingFunction<T>&);
    void findNextMove(const T&);

 private:
    Node root;
    const RatingFunction<T>& h_function;
    const RatingFunction<T>& u_function;

    double update(Node&, unsigned);
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
    std::function<T(T,T)> fn;
    Type nextType;
};

#include "MiniMaxTree.ipp"

#endif /* MINI_MAX_TREE_HPP */
