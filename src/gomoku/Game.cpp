/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "gomoku/Game.hpp"
#include "gomoku/Match.hpp"
#include "gomoku/MatchGraphics.hpp"
#include "gomoku/MatchInput.hpp"
#include "macros.hpp"

namespace gomoku {
    Game::Game()
     : mbe::Game(std::unique_ptr<Game::State>(
        new Match(std::unique_ptr<Match::Graphics>(new MatchGraphics()), 
            std::unique_ptr<Match::InputHandler>(new MatchInput())))) { }

    void Game::update() {

    }

    void Game::updateGraphics(Renderer& graphics) {

    }

    void Game::processInput(Input& input) {

    }
}
