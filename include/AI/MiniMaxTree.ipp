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
template<typename Generator>
typename MiniMaxTree<T>::AnalysisReport MiniMaxTree<T>::analyze(const T& currentState) {
    root = Node{currentState, Type::MAX};
    auto& bestNode = update<Generator>(root, AITraits::MAX_DEPTH);
    TRACE(bestNode.value);
    return {base::Command<T>(), AITraits::MAX_DEPTH};
}

template<typename T>
template<typename Generator>
typename MiniMaxTree<T>::Node& MiniMaxTree<T>::update(Node& node, unsigned depth) {
    T& currentState = node.state;
    bool over = currentState.over();
    bool exceededMaxDepth = (depth == 0);
    if (over || exceededMaxDepth) {
        auto fn = over ? u_function : h_function;
        node.value = fn(node.state);
        return node;
    }

    std::unordered_map<Type, initial_values<double>> options;
    std::function<double(double, double)> nop;
    options[Type::MAX] = {-INT_MAX, &node.alpha, nop, Type::MIN};
    options[Type::MIN] = {INT_MAX, &node.beta, nop, Type::MAX};
    options[Type::MAX].updater = [](double a, double b) { return std::max(a, b); };
    options[Type::MIN].updater = [](double a, double b) { return std::min(a, b); };

    double best = options[node.type].best;
    double& param = *options[node.type].param;
    auto updater = options[node.type].updater;
    Type nextType = options[node.type].nextType;

    Generator generator(currentState);
    while (generator.hasNext()) {
    // for (unsigned i = 0; i < 2; i++) {
        T next = generator.generateNext();
        Node nextNode{next, nextType};

        auto& bestChild = update<Generator>(nextNode, depth - 1);
        best = updater(best, bestChild.value);
        param = updater(param, best);

        if (best == bestChild.value) {
            // node.bestChild = &bestChild;
            node.state = bestChild.state;
        }

        double alpha = nextNode.alpha;
        double beta = nextNode.beta;
        node.children.push_back(std::move(nextNode));
        if (beta <= alpha) {
            break;
        }
    }

    // return best;
    // return *node.bestChild;
    node.value = best;
    return node;
}
