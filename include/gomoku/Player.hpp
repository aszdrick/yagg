/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_PLAYER_HPP
#define GOMOKU_PLAYER_HPP

#include <memory>
#include "base/Component.hpp"
#include "Command.hpp"
#include "Gomoku.hpp"

class Gomoku::Player : public BasePlayer {
    class Graphics;
    class InputHandler;
 public:
    using GraphicalComponent = base::Component<Player, Renderer>;
    using InputComponent = base::Component<go::Position, Input>;

    Player(Graphics* const = new Graphics(),
           InputHandler* const = new InputHandler());

 private:
    std::unique_ptr<Graphics> graphicsPtr;
    std::unique_ptr<InputHandler> inputPtr;
    Graphics& graphics;
    InputHandler& input;

    void onUpdateRenderer(Renderer&) override;
    Move onProcessInput(Input&) override;

    class Graphics : public GraphicalComponent {
     private:
        void doUpdate(Agent&, Element&) override;
    };

    class InputHandler : public InputComponent {
     private:
        void doUpdate(Agent&, Element&) override;
    };
};

#endif /* GOMOKU_PLAYER_HPP */
