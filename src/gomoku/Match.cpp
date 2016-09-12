/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <type_traits>
#include <utility>
#include "gomoku/Traits.hpp"
#include "gomoku/Match.hpp"
#include "multimedia/MatchGraphics.hpp"
#include "multimedia/MatchHandler.hpp"
#include "extra/macros.hpp"

//------------------------------- Match class -------------------------------//

Gomoku::Match::Match(Player&& p1, Player&& p2)
: graphicsPtr(new Graphics()), inputPtr(new Handler()),
  graphics(*graphicsPtr), input(*inputPtr),
  players{std::move(p1), std::move(p2)} {
    players[0].setTeam(go::Team::BLACK);
    players[1].setTeam(go::Team::WHITE);
}

void Gomoku::Match::updatePlayers(Player::Input& positions) {
    auto move = players[state.currentPlayer()].processInput(state, positions);
    move.execute(state);
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

bool Gomoku::Match::isOver() const {
    return state.isOver();
}

void Gomoku::Match::onUpdateRenderer(Renderer& render) {
    graphics.update(*this, render);
}

Gomoku::Match::Response Gomoku::Match::onProcessInput(Input& in) {
    input.update(*this, in);
    return { Response::Type::SELF, 0, this};
}
