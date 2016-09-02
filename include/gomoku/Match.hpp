/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_HPP
#define GOMOKU_MATCH_HPP

#include <array>
#include "base/Component.hpp"
#include "Gomoku.hpp"
#include "Player.hpp"
#include "State.hpp"

class Gomoku::Match : public State {
    class Graphics;
    class InputHandler;
 public:
    using GraphicalComponent = base::Component<Match, Renderer>;
    using InputComponent = base::Component<Match, Input>;

    enum class Type {
        PLAYER_VS_BOT, PLAYERS_ONLY, BOTS_ONLY
    };

 public:
    Match(Player&&,
          Player&&,
          Graphics* const = new Graphics(),
          InputHandler* const = new InputHandler());
    
    void handleEvents(Player::Input&);

 private:
    std::unique_ptr<Graphics> graphicsPtr;
    std::unique_ptr<InputHandler> inputPtr;
    GraphicalComponent& graphics;
    InputComponent& input;
    std::array<Player,2> players;
    short currentPlayer;
    go::State state;

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
        go::Position handleMousePressed(float, float);
    };
};

#endif /* GOMOKU_MATCH_HPP */
