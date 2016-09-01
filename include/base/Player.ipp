/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template<typename R, typename E>
void Player<R,E>::syncUpdate() {
    update();
}

template<typename R, typename E>
void Player<R,E>::updateRenderer(R& renderer) {
    updateGraphics(renderer);
}

template<typename R, typename E>
void Player<R,E>::processEvents(E& events) {
    processInput(events);
}

template<typename R, typename E>
void Player<R,E>::update() { }
