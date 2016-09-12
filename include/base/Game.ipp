/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template<typename I, typename R>
base::Game<I,R>::Game(State* const initial)
 : current(*initial), _closed(false) {
    states.emplace_front(std::move(initial));
    currentIt = states.begin();
}

template<typename I, typename R>
bool base::Game<I,R>::close() {
    if (!_closed) {
        _closed = onClose();
    }
    return _closed;
}

template<typename I, typename R>
void base::Game<I,R>::periodicUpdate() {
    if (!_closed) {
        current.get().periodicUpdate();
        onPeriodicUpdate();
    }
}

template<typename I, typename R>
void base::Game<I,R>::updateRenderer(R& renderer) {
    if (!_closed) {
        current.get().updateRenderer(renderer);
        onUpdateRenderer(renderer);
    }
}

template<typename I, typename R>
void base::Game<I,R>::processInput(I& provider) {
    if (!_closed) {
        auto transition = current.get().processInput(provider);
        
        switch(transition.type) {
            case State::Response::Type::SELF:
                break;
            case State::Response::Type::CLOSE:
                close();
                break;
            case State::Response::Type::MOVE:
                move(transition);
                break;
            case State::Response::Type::REPLACE:
                replace(transition);
                break;
            case State::Response::Type::RESTORE:
                restore(transition);
                break;
            case State::Response::Type::STORE:
                store(transition);
                break;
        }

        onProcessInput(provider);
    }
}

template<typename I, typename R>
bool base::Game<I,R>::onClose() {
    return true;
}

template<typename I, typename R>
void base::Game<I,R>::onPeriodicUpdate() { }

template<typename I, typename R>
void base::Game<I,R>::onUpdateRenderer(R&) { }

template<typename I, typename R>
void base::Game<I,R>::onProcessInput(I&) { }

template<typename I, typename R>
bool base::Game<I,R>::closed() {
    return _closed;
}

template<typename I, typename R>
void base::Game<I,R>::move(const typename State::Response& transition) {
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

template<typename I, typename R>
void base::Game<I,R>::replace(const typename State::Response& transition) {
    popState();
    pushState(transition.state);
    _closed = false;
}

template<typename I, typename R>
void base::Game<I,R>::restore(const typename State::Response& transition) {
    if (!states.empty()) {
        auto removesCount = transition.offset;
        while (removesCount > 0) {
            popState();
            --removesCount;
        }
    }
}

template<typename I, typename R>
void base::Game<I,R>::store(const typename State::Response& transition) {
    if (transition.offset == 0) {
        pushState(transition.state);
    } else {
        move(transition);
        pushState(transition.state);
    }
}

template<typename I, typename R>
void base::Game<I,R>::popState() {
    currentIt = states.erase(currentIt);
    if (!states.empty()) {
        current = *states.front();
    } else {
        close();
    }
}

template<typename I, typename R>
void base::Game<I,R>::pushState(State* const state) {
    currentIt = states.emplace(currentIt, std::move(state));
    current = **currentIt;
}