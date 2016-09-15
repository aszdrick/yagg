/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template <typename I, typename R>
void base::GameState<I,R>::periodicUpdate() {
    onPeriodicUpdate();
}
    
template <typename I, typename R>
void base::GameState<I,R>::updateRenderer(R& renderer) {
    onUpdateRenderer(renderer);
}
    
template <typename I, typename R>
typename base::GameState<I,R>::Response 
base::GameState<I,R>::processInput(I& input) {
    return onProcessInput(input);
}

template <typename I, typename R>
void base::GameState<I,R>::resize(double width, double height) {
    onResize(width, height);
}

template <typename I, typename R>
void base::GameState<I,R>::onPeriodicUpdate() { }

template <typename I, typename R>
void base::GameState<I,R>::onUpdateRenderer(R&) { }

template <typename I, typename R>
void base::GameState<I,R>::onResize(double, double) { }

template <typename I, typename R>
typename base::GameState<I,R>::Response
base::GameState<I,R>::onProcessInput(I&) {
    return {Response::Type::SELF, 0, nullptr};
}
