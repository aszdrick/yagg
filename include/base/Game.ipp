/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "GameState.hpp"

template <typename R, typename E>
Game<R,E>::Game(State* const initial)
 : current(*initial), _closed(false) {
    states.emplace_front(std::move(initial));
}

template <typename R, typename E>
bool Game<R,E>::close() {
    if (!_closed) {
        _closed = onClose();
    }
    return _closed;
}

template <typename R, typename E>
bool Game<R,E>::onClose() {
    return true;
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
    current.get().updateRenderer(renderer);
    updateGraphics(renderer);
}

template <typename R, typename E>
void Game<R,E>::processEvents(E& provider) {
    auto transition = current.get().processEvents(provider);
    
    switch(transition.type) {
        case State::Transition::Type::SELF:
            break;
        case State::Transition::Type::STORE:
            pushState(transition.state);
            break;
        case State::Transition::Type::REPLACE:
            replaceState(transition.state);
            break;
        case State::Transition::Type::RESTORE:
            popState();
            break;
        case State::Transition::Type::CLOSE:
            close();
            break;
    }

    processInput(provider);
}

template <typename R, typename E>
void Game<R,E>::syncUpdate() {
    current.get().syncUpdate();
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

template<typename R, typename E>
void Game<R,E>::replaceState(State* const state) {
    popState();
    pushState(state);
}