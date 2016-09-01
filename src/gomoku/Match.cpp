/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <type_traits>
#include <utility>
#include "gomoku/Match.hpp"

namespace gomoku {
    Match::Match(Graphics* const graphics, InputHandler* const input,
                 Player&& p1, Player&& p2)
    : graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
      graphics(*graphicsPtr), input(*inputPtr),
      players{std::move(p1), std::move(p2)} { }

    void Match::handleEvents(Player::Input& events) {
        players[0].processEvents(events);
        auto move = players[0].pendingMove();
        if (move.valid) {
            state.addStone(move.coords, order[currentPlayer]);
            currentPlayer = 1 - currentPlayer;
        }
    }

    void Match::update() {

    }

    void Match::updateGraphics(Game::Renderer& render) {
        graphics.update(*this, render);
    }

    Match::Transition Match::processInput(Game::Input& in) {
        input.update(*this, in);
        return { Transition::Type::SELF, this};
    }


}