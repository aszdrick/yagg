/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template <typename R, typename E>
void GameState<R, E>::updateLogic() {
    update();    
}
    
template <typename R, typename E>
void GameState<R, E>::updateRenderer(R& renderer) {
    updateGraphics(renderer);
}
    
template <typename R, typename E>
void GameState<R, E>::processEvents(E& input) {
    processInput(input);
}
