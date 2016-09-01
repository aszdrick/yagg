/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include <type_traits>
#include <utility>
#include "gomoku/Match.hpp"

namespace gomoku {
    Match::Match(std::unique_ptr<Graphics>&& g, std::unique_ptr<InputHandler>&& i)
    : graphicsPtr(std::forward<std::decay<decltype(g)>::type>(g)),
      inputPtr(std::forward<std::decay<decltype(i)>::type>(i)),
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