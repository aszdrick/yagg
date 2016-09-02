/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template <typename R, typename I>
Game<R,I>::Game(State* const initial)
 : current(*initial), _closed(false) {
    states.emplace_front(std::move(initial));
}

template <typename R, typename I>
bool Game<R,I>::close() {
    if (!_closed) {
        _closed = onClose();
    }
    return _closed;
}

template <typename R, typename I>
void Game<R,I>::periodicUpdate() {
    current.get().periodicUpdate();
    onPeriodicUpdate();
}

template <typename R, typename I>
void Game<R,I>::updateRenderer(R& renderer) {
    current.get().updateRenderer(renderer);
    onUpdateRenderer(renderer);
}


template <typename R, typename I>
void Game<R,I>::processInput(I& provider) {
    auto transition = current.get().processInput(provider);
    
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

    onProcessInput(provider);
}

template <typename R, typename I>
bool Game<R,I>::onClose() { return true; }

template <typename R, typename I>
void Game<R,I>::onPeriodicUpdate() { }

template <typename R, typename I>
void Game<R,I>::onUpdateRenderer(R&) { }

template <typename R, typename I>
void Game<R,I>::onProcessInput(I&) { }

template <typename R, typename I>
bool Game<R,I>::closed() {
    return _closed;
}

template <typename R, typename I>
void Game<R,I>::popState() {
    states.pop_front();
    if (!states.empty()) {
        current = *states.front();
    } else {
        close();
    }
}

template <typename R, typename I>
void Game<R,I>::pushState(State* const state) {
    states.emplace_front(std::move(state));
    current = *states.front();
}

template<typename R, typename I>
void Game<R,I>::replaceState(State* const state) {
    popState();
    pushState(state);
}