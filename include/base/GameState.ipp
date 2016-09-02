/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template <typename R, typename I>
void base::GameState<R,I>::periodicUpdate() {
    onPeriodicUpdate();
}
    
template <typename R, typename I>
void base::GameState<R,I>::updateRenderer(R& renderer) {
    onUpdateRenderer(renderer);
}
    
template <typename R, typename I>
typename base::GameState<R,I>::Transition 
base::GameState<R,I>::processInput(I& input) {
    return onProcessInput(input);
}

template <typename R, typename I>
void base::GameState<R,I>::onPeriodicUpdate() { }

template <typename R, typename I>
void base::GameState<R,I>::onUpdateRenderer(R&) { }

template <typename R, typename I>
typename base::GameState<R,I>::Transition
base::GameState<R,I>::onProcessInput(I&) {
    return {Transition::Type::SELF, nullptr};
}