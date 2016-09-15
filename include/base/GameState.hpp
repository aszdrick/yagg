/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BASE_GAME_STATE_HPP
#define BASE_GAME_STATE_HPP

template<typename I, typename R>
class Game;

namespace base {
    template <typename I, typename R>
    class GameState {
     public:
        struct Response;
        using Renderer = R;
        using Input = I;

        void periodicUpdate();
        void updateRenderer(Renderer&);
        Response processInput(Input&);
        void resize(double, double);

     private:
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&);
        virtual Response onProcessInput(Input&);
        virtual void onResize(double, double);
    };

    template <typename I, typename R>
    struct GameState<I,R>::Response {
        enum class Type { 
            CLOSE,
            MOVE,
            POP,
            PUSH,
            REPLACE,
            SELF
        };

        Type type;
        int offset;
        GameState<I,R>* state;
    };    
}

#include "GameState.ipp"

#endif /* BASE_GAME_STATE_HPP */
