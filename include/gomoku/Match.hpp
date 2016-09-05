/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

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
    
    void updatePlayers(Player::Input&);
    void restart();
    go::Team currentTeam() const;
    bool isOver() const;

 private:
    std::unique_ptr<Graphics> graphicsPtr;
    std::unique_ptr<InputHandler> inputPtr;
    GraphicalComponent& graphics;
    InputComponent& input;
    std::array<Player,2> players;
    go::State state;

    void onUpdateRenderer(Renderer&) override;
    Transition onProcessInput(Input&) override;

    class InputHandler : public InputComponent {
     private:
        void doUpdate(Agent&, Element&) override;
        go::Position pixelToPosition(const gm::Pixel&);
        bool isInsideBoard(const gm::Pixel&);
    };

    class Graphics : public GraphicalComponent {
     public:
        Graphics();
     private:
        sf::Font font;
        void doUpdate(Agent&, Element&) override;
        void drawBoard(Element&) const;
        void drawStones(Agent&, Element&) const;
        void drawGameOverScreen(Agent&, Element&) const;
        void highlight(Agent&, Element&) const;
    };

};

#endif /* GOMOKU_MATCH_HPP */
