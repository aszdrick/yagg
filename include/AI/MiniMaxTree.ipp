/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */
#include <unordered_map>
// #include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

template<typename T>
MiniMaxTree<T>::MiniMaxTree(const RatingFunction<T>& heuristic,
    const RatingFunction<T>& utility)
 : h_function(heuristic),
   u_function(utility) {

}

template<typename T>
void MiniMaxTree<T>::findNextMove(const T& currentState) {
    root = Node{currentState, Type::MAX};
    // update(root, 5);
}

template<typename T>
double MiniMaxTree<T>::update(Node& node, unsigned depth) {
    T& currentState = node.state;
    if (depth == 0 || currentState.isOver()) {
        return node.value;
    }

    std::unordered_map<Type, initial_values<double>> options;
    options[Type::MAX] = {-INT_MAX, &node.alpha, std::max<double>, Type::MIN};
    options[Type::MIN] = {INT_MAX, &node.beta, std::min<double>, Type::MAX};

    double best = options[node.type].best;
    double& param = *options[node.type].param;
    auto fn = options[node.type].fn;
    Type nextType = options[node.type].nextType;

    // while (currentState.hasNext()) {
    for (unsigned i = 0; i < 2; i++) {
        T next = currentState.generateNext();
        Node nextNode{next, nextType};

        best = fn(best, update(nextNode, depth - 1));
        param = fn(param, best);
        double alpha = nextNode.alpha;
        double beta = nextNode.beta;

        node.children.push_back(std::move(nextNode));
        if (beta <= alpha) {
            break;
        }
    }

    return best;
}
