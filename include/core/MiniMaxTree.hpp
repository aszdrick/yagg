/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef MINI_MAX_TREE_HPP
#define MINI_MAX_TREE_HPP

#include "Command.hpp"
#include <functional>
#include <memory>
#include <vector>

template<typename T>
class MiniMaxTree {
    using RatingFunction = std::function<double(const T&)>;
    struct Node;
    enum class Type { MIN, MAX };

 public:
    MiniMaxTree(const RatingFunction&, const RatingFunction&);
    Command<T> findNextMove(const T&);

 private:
    std::unique_ptr<Node> root;
    const RatingFunction& u_function;
    const RatingFunction& h_function;
};

template<typename T>
struct MiniMaxTree<T>::Node {
    T state;
    double value;
    double alpha;
    double beta;
    std::vector<std::unique_ptr<Node>> children;
};

#endif /* MINI_MAX_TREE_HPP */
