/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <type_traits>
#include <utility>
#include "gomoku/Traits.hpp"
#include "gomoku/Match.hpp"
#include "multimedia/MatchGraphics.hpp"
#include "multimedia/MatchHandler.hpp"
#include "extra/macros.hpp"

Match::Match(Player&& p1, Player&& p2)
: graphicsPtr(new Graphics()), inputPtr(new Handler()),
  graphics(*graphicsPtr), input(*inputPtr),
  players{std::move(p1), std::move(p2)} {
    players[0].setTeam(go::Team::BLACK);
    players[1].setTeam(go::Team::WHITE);
}

void Match::updatePlayers(go::Position& position) {
    if (!over()) {
        auto move = players[state.currentPlayer()].makeMove(state, position);
        if (move.isValid()) {
            move.execute(state);
            moveIterations = move.iterations();
        }
    }
}

void Match::restart() {
    state = go::State();
}

go::Team Match::currentTeam() const {
    return players[state.currentPlayer()].getTeam();
}

go::Team Match::winnerTeam() const {
    return players[state.winnerPlayer()].getTeam();
}

bool Match::over() const {
    return state.over();
}

bool Match::hasWinner() const {
    return state.hasWinner();
}

bool Match::full() const {
    return state.full();
}

unsigned Match::iterations() const {
    return moveIterations;
}

void Match::onUpdateRenderer(Renderer& render) {
    graphics.update(*this, render);
}

Match::Response Match::onProcessInput(InputProcessor& processor, Input& list) {
    static const auto inv = MatchTraits::SUP_BOARD_LIMIAR + go::Position{1, 1};
    static auto request = Request{Request::Type::NONE, inv};

    for (auto event : list) {
        request = input.update(processor, event);
        switch (request.type) {
            case Request::Type::NONE:
                break;
            case Request::Type::PAUSE:
                restart();
                return {Response::Type::SELF, 0, nullptr};
            case Request::Type::PLAY:
                updatePlayers(request.position);
                return {Response::Type::SELF, 0, nullptr};
            case Request::Type::UNDO:
                // state.undo();
                return {Response::Type::SELF, 0, nullptr};
        }
    }
    updatePlayers(request.position);
    return {Response::Type::SELF, 0, nullptr};
}
