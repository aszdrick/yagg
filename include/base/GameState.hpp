/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BASE_GAME_STATE_HPP
#define BASE_GAME_STATE_HPP

namespace base {
    template <typename I, typename R>
    class GameState {
     public:
        struct Transition;
        using Renderer = R;
        using Input = I;

        void periodicUpdate();
        void updateRenderer(Renderer&);
        Transition processInput(Input&);

     private:
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&);
        virtual Transition onProcessInput(Input&);
    };

    template <typename I, typename R>
    struct GameState<I,R>::Transition {
        enum class Type { SELF, STORE, REPLACE, RESTORE, CLOSE };
        Type type;
        GameState<I,R>* state;
    };    
}

#include "GameState.ipp"

#endif /* BASE_GAME_STATE_HPP */