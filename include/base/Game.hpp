/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <memory>
#include "GameState.hpp"

namespace base {
    template <typename R, typename I>
    class Game {
     public:
        using Renderer = R;
        using Input = I;
        using State = GameState<R, I>;

        bool close();
        bool closed();
        void periodicUpdate();
        void updateRenderer(Renderer&);
        void processInput(Input&);

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
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&);
        virtual void onProcessInput(Input&);
    };
}

#include "Game.ipp"

#endif /* GAME_HPP */