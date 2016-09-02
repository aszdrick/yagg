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
        class Graphics;
        class InputHandler;
     public:
        using GraphicalComponent = Component<Match, Renderer>;
        using InputComponent = Component<Match, Input>;

        Match(Player&&,
              Player&&,
              Graphics* const = new Graphics(),
              InputHandler* const = new InputHandler());
        
        void handleEvents(Player::Input&);
        const State& getState() const {
            return state;
        }

     private:
        std::unique_ptr<Graphics> graphicsPtr;
        std::unique_ptr<InputHandler> inputPtr;
        GraphicalComponent& graphics;
        InputComponent& input;
        std::array<Player,2> players;
        const std::array<Team, 2> order = {Team::BLACK, Team::WHITE};
        State state;
        short currentPlayer;

        void onUpdateRenderer(Renderer&) override;
        Transition onProcessInput(Input&) override;

        class Graphics : public GraphicalComponent {
         private:
            void doUpdate(Agent&, Element&) override;
            void drawBoard(Element&) const;
            void drawBalls(Agent&, Element&) const;
        };

        class InputHandler : public InputComponent {
         private:
            void doUpdate(Agent&, Element&) override;
            Game::PlayerInput handleMousePressed(float, float);
        };

    };
}



#endif /* GOMOKU_MATCH_HPP */
