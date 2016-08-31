/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_HPP
#define GOMOKU_MATCH_HPP

#include "Game.hpp"
#include "base/Component.hpp"

namespace gomoku {
    enum class Team {
        BLACK, WHITE
    };

    enum class MatchType {
        PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
    };

    class Match : public Game::State {
     public:
        using Graphics = Component<Match, Game::Renderer>;
        using InputHandler = Component<Match, Game::Input>; 

        Match(std::unique_ptr<Graphics>, std::unique_ptr<InputHandler>);

     private:
        std::unique_ptr<Graphics> graphicsPtr;
        std::unique_ptr<InputHandler> inputPtr;
        Graphics& graphics;
        InputHandler& input;

        void update() override;
        void updateGraphics(Game::Renderer&) override;
        void processInput(Game::Input&) override;
    };    
 
    struct Stone {
        Team team;
        unsigned row;
        unsigned column;
    };
}

#endif /* GOMOKU_MATCH_HPP */
