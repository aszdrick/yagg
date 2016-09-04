/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef BASE_PLAYER_HPP
#define BASE_PLAYER_HPP

#include "Command.hpp"

namespace base {
    template<typename Game, typename Entry>
    class Player {
     public:
        using Board = Game;
        using Input = Entry;
        using Move = Command<Game>;

        void periodicUpdate();
        Move processInput(Game&, Entry&);
     private:
        virtual void onPeriodicUpdate();
        virtual Move onProcessInput(Board&, Input&) = 0;
    };
}

#include "Player.ipp"

#endif /* BASE_PLAYER_HPP */