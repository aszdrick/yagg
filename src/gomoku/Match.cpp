/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <type_traits>
#include <utility>
#include "gomoku/Traits.hpp"
#include "gomoku/Match.hpp"
#include "multimedia/MatchGraphics.hpp"
#include "multimedia/MatchHandler.hpp"
#include "extra/macros.hpp"

Gomoku::Match::Match(Player&& p1, Player&& p2)
: graphicsPtr(new Graphics()), inputPtr(new Handler()),
  graphics(*graphicsPtr), input(*inputPtr),
  players{std::move(p1), std::move(p2)} {
    players[0].setTeam(go::Team::BLACK);
    players[1].setTeam(go::Team::WHITE);
}

void Gomoku::Match::updatePlayers(Player::Input& positions) {
    if (!over()) {
        auto move = players[state.currentPlayer()].processInput(state, positions);
        if (move.isValid()) {
            move.execute(state);
            moveIterations = move.iterations();
        }
    }
}

void Gomoku::Match::restart() {
    state = go::State();
}

go::Team Gomoku::Match::currentTeam() const {
    return players[state.currentPlayer()].getTeam();
}

go::Team Gomoku::Match::winnerTeam() const {
    return players[state.winnerPlayer()].getTeam();
}

bool Gomoku::Match::over() const {
    return state.over();
}

bool Gomoku::Match::hasWinner() const {
    return state.hasWinner();
}

bool Gomoku::Match::full() const {
    return state.full();
}

unsigned Gomoku::Match::iterations() const {
    return moveIterations;
}

void Gomoku::Match::onUpdateRenderer(Renderer& render) {
    graphics.update(*this, render);
}

Gomoku::Match::Response Gomoku::Match::onProcessInput(Input& in) {
    input.update(*this, in);
    return { Response::Type::SELF, 0, this};
}
