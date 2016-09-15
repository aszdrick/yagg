/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template <typename I, typename R, typename A>
void base::GameState<I,R,A>::periodicUpdate() {
    onPeriodicUpdate();
}
    
template <typename I, typename R, typename A>
void base::GameState<I,R,A>::updateRenderer(R& renderer) {
    onUpdateRenderer(renderer);
}
    
template <typename I, typename R, typename A>
typename base::GameState<I,R,A>::Response 
base::GameState<I,R,A>::processInput(A& processor, I& input) {
    return onProcessInput(processor, input);
}

template <typename I, typename R, typename A>
void base::GameState<I,R,A>::resize(double width, double height) {
    onResize(width, height);
}

template <typename I, typename R, typename A>
void base::GameState<I,R,A>::onPeriodicUpdate() { }

template <typename I, typename R, typename A>
void base::GameState<I,R,A>::onUpdateRenderer(R&) { }

template <typename I, typename R, typename A>
void base::GameState<I,R,A>::onResize(double, double) { }

template <typename I, typename R, typename A>
typename base::GameState<I,R,A>::Response
base::GameState<I,R,A>::onProcessInput(A&, I&) {
    return {Response::Type::SELF, 0, nullptr};
}
