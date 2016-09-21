/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template<typename I, typename R, typename A>
base::Game<I,R,A>::Game(State* const initial)
 : current(*initial) {
    states.emplace_front(std::move(initial));
    currentIt = states.begin();
}

template<typename I, typename R, typename A>
bool base::Game<I,R,A>::close() {
    if (!_closed) {
        _closed = onClose();
    }
    return _closed;
}

template<typename I, typename R, typename A>
bool base::Game<I,R,A>::switchScreenModeRequested() {
    bool old = switchMode;
    switchMode = false;
    return old;
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::switchScreenMode() {
    switchMode = !switchMode;
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::periodicUpdate() {
    if (!_closed) {
        current.get().periodicUpdate();
        onPeriodicUpdate();
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::updateRenderer(R& renderer) {
    if (!_closed) {
        current.get().updateRenderer(renderer);
        onUpdateRenderer(renderer);
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::processInput(A& processor, I& provider) {
    onProcessInput(processor, provider);
    
    if (!_closed) {
        auto transition = current.get().processInput(processor, provider);
        
        switch(transition.type) {
            case State::Response::Type::SELF:
                break;
            case State::Response::Type::CLOSE:
                close();
                break;
            case State::Response::Type::MOVE:
                move(transition);
                break;
            case State::Response::Type::POP:
                pop(transition);
                break;
            case State::Response::Type::PUSH:
                push(transition);
                break;
            case State::Response::Type::REPLACE:
                replace(transition);
                break;
            case State::Response::Type::PAUSE:
                pauseGame();
                break;
        }
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::pauseGame() {
    onPauseGame();
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::onPauseGame() { }

template<typename I, typename R, typename A>
void base::Game<I,R,A>::resize(double width, double height) {
    current.get().resize(width, height);
    onResize(width, height);
}

template<typename I, typename R, typename A>
bool base::Game<I,R,A>::onClose() {
    return true;
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::onPeriodicUpdate() { }

template<typename I, typename R, typename A>
void base::Game<I,R,A>::onUpdateRenderer(R&) { }

template<typename I, typename R, typename A>
void base::Game<I,R,A>::onProcessInput(A&, I&) { }

template<typename I, typename R, typename A>
void base::Game<I,R,A>::onResize(double, double) { }

template<typename I, typename R, typename A>
bool base::Game<I,R,A>::closed() {
    return _closed;
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::move(const typename State::Response& transition) {
    if (!states.empty()) {
        if (transition.offset > 0) {
            auto advancesCount = transition.offset;
            while (advancesCount > 0) {
                if (currentIt == states.end()) {
                    currentIt = std::prev(states.end());
                    break;
                }
                std::next(currentIt);
                --advancesCount;
            }

        } else if (transition.offset < 0) {
            auto retractsCount = transition.offset;
            while (retractsCount < 0) {
                if (currentIt == states.begin()) {
                    break;
                }
                std::prev(currentIt);
                ++retractsCount;
            }
        }
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::replace(const typename State::Response& transition) {
    popState();
    pushState(transition.state);
    _closed = false;
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::pop(const typename State::Response& transition) {
    if (!states.empty()) {
        auto removesCount = transition.offset;
        while (removesCount > 0) {
            popState();
            --removesCount;
        }
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::push(const typename State::Response& transition) {
    if (transition.offset == 0) {
        pushState(transition.state);
    } else {
        move(transition);
        pushState(transition.state);
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::popState() {
    currentIt = states.erase(currentIt);
    if (!states.empty()) {
        current = *states.front();
    } else {
        close();
    }
}

template<typename I, typename R, typename A>
void base::Game<I,R,A>::pushState(State* const state) {
    currentIt = states.emplace(currentIt, std::move(state));
    current = **currentIt;
}