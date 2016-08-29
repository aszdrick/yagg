/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef MINI_MAX_TREE_HPP
#define MINI_MAX_TREE_HPP

#include "Command.hpp"
#include <functional>
#include <memory>
#include <vector>

template<typename T>
using RatingFunction = std::function<double(const T&)>;

template<typename T>
class MiniMaxTree {
    struct Node;
    enum class Type { MIN, MAX };

 public:
    MiniMaxTree(const RatingFunction<T>&, const RatingFunction<T>&);
    Command<T> findNextMove(const T&);

 private:
    std::unique_ptr<Node> root;
    const RatingFunction<T>& h_function;
    const RatingFunction<T>& u_function;
};

template<typename T>
struct MiniMaxTree<T>::Node {
    T state;
    double value;
    double alpha;
    double beta;
    std::vector<std::unique_ptr<Node>> children;
};

#include "MiniMaxTree.ipp"

#endif /* MINI_MAX_TREE_HPP */
