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
    auto depth = AITraits::MAX_DEPTH;
    double bestValue = -INT_MAX;
    T bestState;
    Generator generator(currentState);
    while (generator.hasNext()) {
        const T& next = generator.generateNext();
        auto value = calculate<Generator>(next, depth - 1);
        if (value > bestValue) {
            bestValue = value;
            bestState = next;
        }
    }

    return {generator.command(bestState), AITraits::MAX_DEPTH};
}

template<typename T>
template<typename Generator>
double MiniMaxTree<T>::calculate(const T& state, unsigned depth,
    double alpha, double beta, Type type) const {

    bool over = state.over();
    bool exceededMaxDepth = (depth == 0);
    if (over || exceededMaxDepth) {
        auto fn = over ? u_function : h_function;
        return fn(state);
    }

    std::unordered_map<Type, initial_values<double>> options;
    std::function<double(double, double)> nop;
    options[Type::MAX] = {-INT_MAX, &alpha, nop, Type::MIN};
    options[Type::MIN] = {INT_MAX, &beta, nop, Type::MAX};
    options[Type::MAX].updater = [](double a, double b) { return std::max(a, b); };
    options[Type::MIN].updater = [](double a, double b) { return std::min(a, b); };

    double best = options[type].best;
    double& param = *options[type].param;
    auto updater = options[type].updater;
    Type nextType = options[type].nextType;

    Generator generator(state);
    while (generator.hasNext()) {
        T next = generator.generateNext();
        auto nextValue = calculate<Generator>(next, depth - 1, alpha, beta, nextType);
        best = updater(best, nextValue);
        param = updater(param, best);
        if (beta <= alpha) {
            break;
        }
    }
    return best;
}
