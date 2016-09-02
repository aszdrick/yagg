/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Player.hpp"
#include "macros.hpp"

gomoku::Player::Player(Graphics* const graphics, InputHandler* const input)
 : graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
   graphics(*graphicsPtr), input(*inputPtr) { }

gomoku::Move gomoku::Player::pendingMove() const {
    return lastMove;
}

void gomoku::Player::processInput(Input& in) {
    lastMove.valid = false;
    Point move = {-1, -1};
    input.update(move, in);
    if (move != Point{-1, -1}){
        lastMove = {move, true};
    }
}

void gomoku::Player::updateGraphics(Renderer& renderer) {
    graphics.update(*this, renderer);
}

void gomoku::Player::Graphics::doUpdate(Agent& agent, Element& list) {

}

void gomoku::Player::InputHandler::doUpdate(Agent& agent, Element& list) {
    for (auto& event : list) {
        agent = event;
        break;
    }
}