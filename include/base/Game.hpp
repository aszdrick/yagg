/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <memory>
#include "GameState.hpp"

template <typename Render, typename EventProvider>
class Game {
 public:
    using Renderer = Render;
    using Input = EventProvider;
    using State = GameState<Renderer, EventProvider>;

    void updateLogic();
    void updateRenderer(Render&);
    void processEvents(EventProvider&);

 protected:
    Game(State* const);

    State& currentState();

    void pushState(State* const);
    void popState();

 private:
    std::list<State> states;
    State& current;
    
    virtual void update() = 0;
    virtual void updateGraphics(Renderer&) = 0;
    virtual void processInput(Input&) = 0;
};

#include "Game.ipp"

#endif /* GAME_HPP */