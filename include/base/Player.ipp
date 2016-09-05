/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template<typename B, typename I>
void base::Player<B,I>::periodicUpdate() {
    onPeriodicUpdate();
}

template<typename B, typename I>
base::Command<B> base::Player<B,I>::processInput(B& board, I& input) {
    return onProcessInput(board, input);
}

template<typename B, typename I>
void base::Player<B,I>::onPeriodicUpdate() { }
