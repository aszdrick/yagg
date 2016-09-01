/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <memory>

template<typename R, typename E>
class GameState;

template <typename Render, typename EventProvider>
class Game {
 public:
    using Renderer = Render;
    using Input = EventProvider;
    using State = GameState<Renderer, EventProvider>;

    bool close();
    bool closed();
    void syncUpdate();
    void updateRenderer(Render&);
    void processEvents(EventProvider&);

 protected:
    Game(State* const);

    State& currentState();

    void popState();
    void pushState(State* const);
    void replaceState(State* const);

 private:
    std::list<std::unique_ptr<State>> states;
    std::reference_wrapper<State> current;
    bool _closed;
    
    virtual bool onClose();
    virtual void update() = 0;
    virtual void updateGraphics(Renderer&) = 0;
    virtual void processInput(Input&) = 0;
};

#include "Game.ipp"

#endif /* GAME_HPP */