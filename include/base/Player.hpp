/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"

namespace base {
    template<typename Board, typename Render, typename InputProvider>
    class Player {
     public:
        using Renderer = Render;
        using Input = InputProvider;
        using Move = Command<Board>;

        void periodicUpdate();
        void updateRenderer(Render&);
        Move processInput(InputProvider&);
     private:
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&);
        virtual Move onProcessInput(Input&);
    };
}

#include "Player.ipp"

#endif /* PLAYER_HPP */