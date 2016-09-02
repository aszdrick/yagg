/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GO_COMMON_TYPES_HPP
#define GO_COMMON_TYPES_HPP

#include <list>

namespace go {
    enum class Team {
        BLACK, WHITE
    };

    struct Position {
        unsigned row;
        unsigned column;
    };

    struct Stone {
        Position position;
        Team team;
    };

    struct Board {
        std::list<Stone> stones;
    };

    inline bool operator==(const Position& lhs, const Position& rhs) {
        return lhs.row == rhs.row && lhs.column == rhs.column;
    }

    inline bool operator!=(const Position& lhs, const Position& rhs) {
        return !(lhs == rhs);
    }

}

#endif /* GO_COMMON_TYPES_HPP */
