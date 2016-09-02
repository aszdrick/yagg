/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Player.hpp"
#include "extra/macros.hpp"
#include "GomokuTraits.hpp"

Gomoku::Player::Player(Graphics* const graphics, InputHandler* const input)
 : graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
   graphics(*graphicsPtr), input(*inputPtr) { }

Gomoku::Player::Move Gomoku::Player::pendingMove() const {
    return lastMove;
}

void Gomoku::Player::processInput(Input& in) {
    lastMove.valid = false;
    go::Position move = GomokuTraits::INVALID_POSITION;
    input.update(move, in);
    if (move != GomokuTraits::INVALID_POSITION){
        lastMove = {move, true};
    }
}

void Gomoku::Player::updateGraphics(Renderer& renderer) {
    graphics.update(*this, renderer);
}

void Gomoku::Player::Graphics::doUpdate(Agent& agent, Element& list) {

}

void Gomoku::Player::InputHandler::doUpdate(Agent& agent, Element& list) {
    for (auto& event : list) {
        agent = event;
        break;
    }
}