/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */
#include <unordered_map>
#include "gomoku/Traits.hpp"
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
    update(root, AITraits::MAX_DEPTH);
}

template<typename T>
double MiniMaxTree<T>::update(Node& node, unsigned depth) {
    T& currentState = node.state;
    if (depth == 0 || currentState.isOver()) {
        return node.value;
    }

    std::unordered_map<Type, initial_values<double>> options;
    std::function<double(double, double)> nop;
    options[Type::MAX] = {-INT_MAX, &node.alpha, nop, Type::MIN};
    options[Type::MIN] = {INT_MAX, &node.beta, nop, Type::MAX};
    options[Type::MAX].fn = [](double a, double b) { return std::max(a, b); };
    options[Type::MIN].fn = [](double a, double b) { return std::min(a, b); };

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
