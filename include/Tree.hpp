/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef MINI_MAX_TREE_HPP
#define MINI_MAX_TREE_HPP

#include <functional>
#include <memory>

template<typename T>
class MiniMaxTree {
    using RatingFunction = std::function<double(const T&)>;
 public:
    MiniMaxTree(const RatingFunction&, const RatingFunction&);

 private:
    std::unique_ptr<Node> root;
    const RatingFunction& u_function;
    const RatingFunction& h_function;
};

#endif /* MINI_MAX_TREE_HPP */
