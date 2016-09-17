/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */
#include <unordered_map>
#include "gomoku/Traits.hpp"
#include "extra/macros.hpp"

template<typename T, typename... Args>
MiniMaxTree<T, Args...>::MiniMaxTree(const RatingFunction<T, Args...>& heuristic,
    const RatingFunction<T, Args...>& utility)
 : h_function(heuristic),
   u_function(utility) {

}

template<typename T, typename... Args>
template<typename Generator>
minimax::AnalysisReport<T>
MiniMaxTree<T, Args...>::analyze(T& currentState, Args... args) {
    maxDepth = AITraits::MAX_DEPTH;
    double bestValue = -INT_MAX;
    T bestState;
    Generator::reset();
    Generator generator(currentState);
    while (generator.hasNext()) {
        const T& next = generator.generateNext(true);
        auto value = calculate(generator, next, maxDepth - 1, args...);
        if (value > bestValue) {
            bestValue = value;
            bestState = next;
        }
        generator.undo();
    }

    return {generator.command(bestState), Generator::generationCount()};
}

template<typename T, typename... Args>
template<typename Generator>
double MiniMaxTree<T, Args...>::calculate(Generator& generator, const T& state,
    unsigned depth, Args... args) const {

    return exec(generator, state, depth, -INT_MAX, INT_MAX, Type::MIN, args...);
}

template<typename T, typename... Args>
template<typename Generator>
double MiniMaxTree<T, Args...>::exec(Generator& generator, const T& state,
    unsigned depth, double alpha, double beta, Type type, Args... args) const {

    bool over = state.over();
    bool exceededMaxDepth = (depth == 0);
    if (over || exceededMaxDepth) {
        auto fn = over ? u_function : h_function;
        return fn(state, maxDepth - depth, args...);
    }

    std::unordered_map<Type, minimax::initial_values<double>> options;
    std::function<double(double, double)> nop;
    options[Type::MAX] = {-INT_MAX, &alpha, nop, Type::MIN};
    options[Type::MIN] = {INT_MAX, &beta, nop, Type::MAX};
    options[Type::MAX].updater = [](double a, double b) { return std::max(a, b); };
    options[Type::MIN].updater = [](double a, double b) { return std::min(a, b); };

    double best = options[type].best;
    double& param = *options[type].param;
    auto updater = options[type].updater;
    Type nextType = options[type].nextType;

    while (generator.hasNext()) {
        const T& next = generator.generateNext();
        auto nextValue = exec(generator, next, depth - 1, alpha, beta, nextType, args...);
        generator.undo();
        best = updater(best, nextValue);
        param = updater(param, best);
        if (beta <= alpha) {
            break;
        }
    }
    return best;
}
