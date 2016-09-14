/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "extra/macros.hpp"
#include "gomoku/Player.hpp"
#include "gomoku/Traits.hpp"

Player::Player(InputComponent* const input)
 : inputPtr(std::move(input)), input(*inputPtr) {

 }

void Player::setTeam(go::Team t) {
    team = t;
}

go::Team Player::getTeam() const {
    return team;
}

Player::Move Player::onProcessInput(Board& board, Input& in) {
    Move move = input.update(board, in);
    move.setTeam(team);
    return move;
}

Player::Move Player::InputHandler::doUpdate(Agent& board, Element& events) {
    Move move;
    for (auto& event : events) {
        move.setPosition(event);
        break;
    }
    return move;
}
