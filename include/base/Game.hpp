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
        void resize(double, double);
        bool switchScreenModeRequested();

     protected:
        Game(State* const);

        void switchScreenMode();
        State& currentState();

        void popState();
        void pushState(State* const);
        void move(const typename State::Response&);
        void replace(const typename State::Response&);
        void pop(const typename State::Response&);
        void push(const typename State::Response&);

     private:
        std::list<std::unique_ptr<State>> states;
        std::reference_wrapper<State> current;
        typename std::list<std::unique_ptr<State>>::iterator currentIt;
        bool switchMode = false;
        bool _closed = false;

        virtual bool onClose();
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&);
        virtual void onProcessInput(Input&);
        virtual void onResize(double, double);
    };
}

#include "Game.ipp"

#endif /* BASE_GAME_HPP */
