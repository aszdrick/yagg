/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_HPP
#define GOMOKU_MATCH_HPP

#include <array>
#include "base/Component.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "State.hpp"

namespace gomoku {
    enum class MatchType {
        PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
    };

    class Match : public Game::State {
     public:
        using Graphics = Component<Match, Game::Renderer>;
        using InputHandler = Component<Match, Game::Input>; 

        Match(Graphics* const, InputHandler* const, Player&&, Player&&);
        void handleEvents(Player::Input&);
        const State& getState() const {
            return state;
        }

     private:
        std::unique_ptr<Graphics> graphicsPtr;
        std::unique_ptr<InputHandler> inputPtr;
        Graphics& graphics;
        InputHandler& input;
        std::array<Player,2> players;
        const std::array<Team, 2> order = {Team::BLACK, Team::WHITE};
        State state;
        short currentPlayer;

        void update() override;
        void updateGraphics(Game::Renderer&) override;
        Transition processInput(Game::Input&) override;
    };
}



#endif /* GOMOKU_MATCH_HPP */
