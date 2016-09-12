/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "extra/macros.hpp"
#include "gomoku/Player.hpp"
#include "gomoku/Traits.hpp"

Gomoku::Player::Player(InputComponent* const input)
 : inputPtr(std::move(input)), input(*inputPtr) {

 }

void Gomoku::Player::setTeam(go::Team t) {
    team = t;
}

go::Team Gomoku::Player::getTeam() const {
    return team;
}

Gomoku::Player::Move Gomoku::Player::onProcessInput(Board& board, Input& in) {
    Move move = input.update(board, in);
    move.setTeam(team);
    return move;
}

Gomoku::Player::Move Gomoku::Player::InputHandler::doUpdate(Agent& board, Element& events) {
    Move move;
    for (auto& event : events) {
        if (event != GomokuTraits::INVALID_POSITION) {
            move.setPosition(event);
            break;
        }
    }
    return move;
}
