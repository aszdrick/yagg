/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "GameState.hpp"

template <typename R, typename E>
Game<R,E>::Game(std::unique_ptr<State>&& initial)
 : current(*initial) {
    states.push_back(std::move(initial));
}

template <typename R, typename E>
typename Game<R,E>::State& Game<R,E>::currentState() {
    return states.front();
}

template <typename R, typename E>
void Game<R,E>::updateRenderer(R& renderer) {
    current.updateRenderer(renderer);
    updateGraphics(renderer);
}

template <typename R, typename E>
void Game<R,E>::processEvents(E& provider) {
    current.processEvents(provider);
    processInput(provider);
}

template <typename R, typename E>
void Game<R,E>::updateLogic() {
    current.updateLogic();
    update();
}

template <typename R, typename E>
void Game<R,E>::pushState(State* const state) {
    states.emplace_front(state);
}

template <typename R, typename E>
void Game<R,E>::popState() {
    states.pop_front();
}