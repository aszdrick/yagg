/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "GameState.hpp"

template <typename R, typename E>
Game<R,E>::Game(State* const initial)
 : current(*initial), _closed(false) {
    states.emplace_front(std::move(initial));
}

template <typename R, typename E>
void Game<R,E>::close() {
    _closed = true;
}
template <typename R, typename E>
bool Game<R,E>::closed() {
    return _closed;
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
    auto transition = current.processEvents(provider);
    
    switch(transition.type) {
        case State::Transition::Type::SELF:
            break;
        case State::Transition::Type::STASH:
            pushState(transition.state);
            break;
        case State::Transition::Type::REPLACE:
            popState();
            pushState(transition.state);
            break;
        case State::Transition::Type::RESTORE:
            popState();
            break;
    }

    processInput(provider);
}

template <typename R, typename E>
void Game<R,E>::syncUpdate() {
    current.syncUpdate();
    update();
}

template <typename R, typename E>
void Game<R,E>::pushState(State* const state) {
    states.emplace_front(std::move(state));
    current = *states.front();
}

template <typename R, typename E>
void Game<R,E>::popState() {
    states.pop_front();
    if (!states.empty()) {
        current = *states.front();
    } else {
        close();
    }
}