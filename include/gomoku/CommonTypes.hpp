/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GO_COMMON_TYPES_HPP
#define GO_COMMON_TYPES_HPP

#include <list>
#include <ostream>

namespace go {
    enum class Team : unsigned short {
        BLACK = 0, WHITE = 1
    };

    struct Position {
        int row;
        int column;
        static unsigned distance(const go::Position& first,
                                 const go::Position& second) {
            unsigned r1 = first.row;
            unsigned r2 = second.row;
            unsigned c1 = first.column;
            unsigned c2 = second.column;
            auto deltaRow = (r1 > r2) ? r1 - r2 : r2 - r1;
            auto deltaColumn = (c1 > c2) ? c1 - c2 : c2 - c1;
            if (deltaRow == deltaColumn) {
                // same diagonal
                return deltaRow;
            }
        
            // manhattan distance
            return deltaRow + deltaColumn;
        }
    };

    struct Stone {
        Position position;
        Team team;
    };

    class State;

    inline bool operator<(const Position& lhs, const Position& rhs) {
        return lhs.row < rhs.row || lhs.column < rhs.column;
    }

    inline bool operator>(const Position& lhs, const Position& rhs) {
        return rhs < lhs;
    }

    inline bool operator<=(const Position& lhs, const Position& rhs) {
        return !(lhs > rhs);
    }

    inline bool operator>=(const Position& lhs, const Position& rhs) {
        return !(lhs < rhs);
    }

    inline Position operator+(const Position& lhs, const Position& rhs) {
        return {lhs.row + rhs.row, lhs.column + rhs.column};
    }

    inline Position operator-(const Position& lhs, const Position& rhs) {
        return {lhs.row - rhs.row, lhs.column - rhs.column};
    }

    inline Position operator/(const Position& lhs, int rhs) {
        return {lhs.row / rhs, lhs.column / rhs};
    }

    inline bool operator==(const Position& lhs, const Position& rhs) {
        return lhs.row == rhs.row && lhs.column == rhs.column;
    }

    inline bool operator!=(const Position& lhs, const Position& rhs) {
        return !(lhs == rhs);
    }
    
    inline std::string to_string(const go::Team& team) {
        std::string out;
        switch (team) {
            case go::Team::BLACK:
                out = "Black";
                break;
            case go::Team::WHITE:
                out = "White";
                break;
        }
        return out;
    }

    inline std::ostream& operator<<(std::ostream& stream, const go::Team& team) {
        switch (team) {
            case go::Team::BLACK:
                stream << "Black";
                break;
            case go::Team::WHITE:
                stream << "White";
                break;
        }
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const go::Stone& stone) {
        auto pos = stone.position;
        return stream << "{" << pos.row << "," << pos.column << "}" << ", "
               << ((stone.team == go::Team::BLACK) ? "BLACK" : "WHITE");
    }
}

namespace std {
    template<>
    struct hash<go::Team> {
        std::size_t operator()(const go::Team& team) const {
            return static_cast<int>(team);
        }
    };

    template<>
    struct hash<go::Position> {
        std::size_t operator()(const go::Position& pos) const {
            // TODO: make the 15 generic
            return pos.row * 15 + pos.column;
        }
    };
}

#endif /* GO_COMMON_TYPES_HPP */
