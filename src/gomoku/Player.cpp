/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "extra/macros.hpp"
#include "gomoku/Player.hpp"
#include "gomoku/Traits.hpp"

Gomoku::Player::Player(go::Team team, InputHandler* const input)
 : team(team), inputPtr(std::move(input)), input(*inputPtr) {

 }

void Gomoku::Player::setTeam(go::Team t) {
    team = t;
}

go::Team Gomoku::Player::getTeam() const {
    return team;
}

Gomoku::Player::Move Gomoku::Player::onProcessInput(Board& board, Input& in) {
    Move move(team);
    input.update(move, in);
    return move;
}

void Gomoku::Player::InputHandler::doUpdate(Agent& move, Element& events) {
    for (auto& event : events) {
        if (event != GomokuTraits::INVALID_POSITION) {
            move.setPosition(event);
            break;
        }
    }
}