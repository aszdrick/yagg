/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef BASE_GAME_HPP
#define BASE_GAME_HPP

#include <list>
#include <memory>
#include "GameState.hpp"

namespace base {
    template <typename I, typename R>
    class Game {
     public:
        using Input = I;
        using Renderer = R;
        using State = GameState<I, R>;

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

#endif /* BASE_GAME_HPP */
