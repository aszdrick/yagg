/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "extra/macros.hpp"
#include "gomoku/Player.hpp"
#include "gomoku/Traits.hpp"

Gomoku::Player::Player(go::Team team,
                       Graphics* const graphics,
                       InputHandler* const input)
 : team(team), graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
   graphics(*graphicsPtr), input(*inputPtr) { }

void Gomoku::Player::setTeam(go::Team t) {
    team = t;
}

Gomoku::Player::Move Gomoku::Player::onProcessInput(Input& in) {
    Move move(team);
    input.update(move, in);
    return move;
}

void Gomoku::Player::onUpdateRenderer(Renderer& renderer) {
    graphics.update(*this, renderer);
}

void Gomoku::Player::Graphics::doUpdate(Agent& agent, Element& events) {

}

void Gomoku::Player::InputHandler::doUpdate(Agent& move, Element& events) {
    for (auto& event : events) {
        if (event != GomokuTraits::INVALID_POSITION){
            move.setPosition(event);
            break;
        }
    }
}