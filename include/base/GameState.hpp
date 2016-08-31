/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

template <typename Renderer, typename EventProvider = Renderer>
class GameState {
 public:
    void updateLogic();
    void updateRenderer(Renderer&);
    void processEvents(EventProvider&);

 private:
    virtual void update() = 0;
    virtual void updateGraphics(Renderer&) = 0;
    virtual void processInput(EventProvider&) = 0;
};

#include "GameState.ipp"

#endif /* GAME_STATE_HPP */