/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

template <typename Renderer, typename EventProvider = Renderer>
class GameState {
 public:
    struct Transition;

    void syncUpdate();
    void updateRenderer(Renderer&);
    Transition processEvents(EventProvider&);

 private:
    virtual void update() = 0;
    virtual void updateGraphics(Renderer&) = 0;
    virtual Transition processInput(EventProvider&) = 0;
};

template <typename R, typename E>
struct GameState<R,E>::Transition {
    enum class Type { SELF, STASH, REPLACE, RESTORE };
    Type type;
    GameState<R,E>* state;
};

#include "GameState.ipp"

#endif /* GAME_STATE_HPP */