/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

template<typename B, typename R, typename I>
void base::Player<B,R,I>::periodicUpdate() {
    onPeriodicUpdate();
}

template<typename B, typename R, typename I>
void base::Player<B,R,I>::updateRenderer(R& renderer) {
    onUpdateRenderer(renderer);
}

template<typename B, typename R, typename I>
typename base::Player<B,R,I>::Move base::Player<B,R,I>::processInput(I& in) {
    onProcessInput(in);
}

template<typename B, typename R, typename I>
void base::Player<B,R,I>::onPeriodicUpdate() { }

template<typename B, typename R, typename I>
void base::Player<B,R,I>::onUpdateRenderer(R&) { }

template<typename B, typename R, typename I>
typename base::Player<B,R,I>::Move base::Player<B,R,I>::onProcessInput(I&) { }
