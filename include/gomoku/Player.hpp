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
    using InputComponent = base::Component<Move, Input>;

    Player(go::Team = go::Team::BLACK,
           InputHandler* const = new InputHandler());

    void setTeam(go::Team);

 private:
    go::Team team;
    std::unique_ptr<InputHandler> inputPtr;
    InputHandler& input;

    Move onProcessInput(Board&, Input&) override;

    class InputHandler : public InputComponent {
     private:
        void doUpdate(Agent&, Element&) override;
    };
};

#endif /* GOMOKU_PLAYER_HPP */
