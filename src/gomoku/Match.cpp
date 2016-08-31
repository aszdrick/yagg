/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Match.hpp"

namespace gomoku {
    Match::Match(std::unique_ptr<Graphics> g, std::unique_ptr<InputHandler> i)
    : graphicsPtr(std::move(g)), inputPtr(std::move(i)),
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