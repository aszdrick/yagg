/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef PLAYER_HPP
#define PLAYER_HPP

template<typename Render, typename EventProvider = Render>
class Player {
 public:
    using Renderer = Render;
    using Input = EventProvider;

    void syncUpdate();
    void updateRenderer(Render&);
    void processEvents(EventProvider&);
 private:
    virtual void update();
    virtual void updateGraphics(Renderer&) = 0;
    virtual void processInput(Input&) = 0;
};

#include "Player.ipp"

#endif /* PLAYER_HPP */