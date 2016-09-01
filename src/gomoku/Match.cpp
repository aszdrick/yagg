/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <type_traits>
#include <utility>
#include "gomoku/Match.hpp"

namespace gomoku {
    Match::Match(Graphics* const graphics, InputHandler* const input)
    : graphicsPtr(std::move(graphics)), inputPtr(std::move(input)),
      graphics(*graphicsPtr), input(*inputPtr) { }

    void Match::update() {

    }

    void Match::updateGraphics(Game::Renderer& render) {
        graphics.update(*this, render);
    }

    void Match::processInput(Game::Input& in) {
        input.update(*this, in);
    }


}