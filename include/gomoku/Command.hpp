/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_COMMAND_HPP
#define GOMOKU_COMMAND_HPP

#include "base/Command.hpp"
#include "CommonTypes.hpp"

namespace base {
    template<>
    class Command<go::State> {
     public:
        Command(go::Team = go::Team::BLACK);

        bool isValid() const;
        void setTeam(const go::Team&);
        void setPosition(const go::Position&);
        void execute(go::State&);
     private:
        go::Team team;
        go::Position position = {0, 0};
        bool valid = false;
    };
}

#endif /* GOMOKU_COMMAND_HPP */
