/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_HPP
#define GOMOKU_MATCH_HPP

#include <array>
#include "base/Component.hpp"
#include "Player.hpp"
#include "State.hpp"

class Match : public mbe::Game::State {
    class Graphics;
    class Handler;
 public:
    struct Request;
    using GraphicalComponent = base::Component<Match, Renderer>;
    using InputComponent = base::Component<InputProcessor, sf::Event, Request>;
    using Transition = Response::Type;

    enum class Type {
        PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
    };

 public:
    Match(Player&&, Player&&);
    
    void updatePlayers(go::Position&);
    void restart();
    go::Team currentTeam() const;
    go::Team winnerTeam() const;
    bool over() const;
    bool hasWinner() const;
    bool full() const;
    unsigned iterations() const;

 private:
    std::unique_ptr<GraphicalComponent> graphicsPtr;
    std::unique_ptr<InputComponent> inputPtr;
    GraphicalComponent& graphics;
    InputComponent& input;

    std::array<Player,2> players;
    go::State state;
    unsigned moveIterations = 0;

    void onUpdateRenderer(Renderer&) override;
    Response onProcessInput(InputProcessor&, Input&) override;
};

struct Match::Request {
    enum class Type { NONE, PAUSE, PLAY };
    Type type;
    go::Position position;   
};

#endif /* GOMOKU_MATCH_HPP */
