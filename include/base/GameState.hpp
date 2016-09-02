/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

template <typename R, typename I>
class GameState {
 public:
    struct Transition;
    using Renderer = R;
    using Input = I;

    void periodicUpdate();
    void updateRenderer(Renderer&);
    Transition processInput(Input&);

 private:
    virtual void onPeriodicUpdate();
    virtual void onUpdateRenderer(Renderer&);
    virtual Transition onProcessInput(Input&);
};

template <typename R, typename I>
struct GameState<R,I>::Transition {
    enum class Type { SELF, STORE, REPLACE, RESTORE, CLOSE };
    Type type;
    GameState<R,I>* state;
};

#include "GameState.ipp"

#endif /* GAME_STATE_HPP */