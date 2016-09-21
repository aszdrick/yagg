/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BASE_GAME_STATE_HPP
#define BASE_GAME_STATE_HPP

namespace base {
    template <typename I, typename R, typename A>
    class GameState {
     public:
        struct Response;
        using Renderer = R;
        using Input = I;
        using InputProcessor = A;

        void periodicUpdate();
        void updateRenderer(Renderer&);
        Response processInput(InputProcessor&, Input&);
        void resize(double, double);

     private:
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&);
        virtual Response onProcessInput(InputProcessor&, Input&);
        virtual void onResize(double, double);
    };

    template <typename I, typename R, typename A>
    struct GameState<I,R,A>::Response {
        enum class Type { 
            CLOSE,
            MOVE,
            POP,
            PUSH,
            REPLACE,
            SELF,
            PAUSE
        };

        Type type;
        int offset;
        GameState<I,R,A>* state;
    };    
}

#include "GameState.ipp"

#endif /* BASE_GAME_STATE_HPP */
