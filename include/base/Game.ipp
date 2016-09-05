/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template <typename I, typename R>
base::Game<I,R>::Game(State* const initial)
 : current(*initial), _closed(false) {
    states.emplace_front(std::move(initial));
}

template <typename I, typename R>
bool base::Game<I,R>::close() {
    if (!_closed) {
        _closed = onClose();
    }
    return _closed;
}

template <typename I, typename R>
void base::Game<I,R>::periodicUpdate() {
    current.get().periodicUpdate();
    onPeriodicUpdate();
}

template <typename I, typename R>
void base::Game<I,R>::updateRenderer(R& renderer) {
    current.get().updateRenderer(renderer);
    onUpdateRenderer(renderer);
}


template <typename I, typename R>
void base::Game<I,R>::processInput(I& provider) {
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

template <typename I, typename R>
bool base::Game<I,R>::onClose() { return true; }

template <typename I, typename R>
void base::Game<I,R>::onPeriodicUpdate() { }

template <typename I, typename R>
void base::Game<I,R>::onUpdateRenderer(R&) { }

template <typename I, typename R>
void base::Game<I,R>::onProcessInput(I&) { }

template <typename I, typename R>
bool base::Game<I,R>::closed() {
    return _closed;
}

template <typename I, typename R>
void base::Game<I,R>::popState() {
    states.pop_front();
    if (!states.empty()) {
        current = *states.front();
    } else {
        close();
    }
}

template <typename I, typename R>
void base::Game<I,R>::pushState(State* const state) {
    states.emplace_front(std::move(state));
    current = *states.front();
}

template<typename I, typename R>
void base::Game<I,R>::replaceState(State* const state) {
    popState();
    pushState(state);
}
