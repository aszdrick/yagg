/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

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
    using InputComponent = base::Component<Board, Input, Move>;

    Player(InputComponent* const = new InputHandler());

    void setTeam(go::Team);
    go::Team getTeam() const;

 private:
    go::Team team = go::Team::BLACK;
    std::unique_ptr<InputComponent> inputPtr;
    InputComponent& input;

    Move onProcessInput(Board&, Input&) override;

    class InputHandler : public InputComponent {
     private:
        Product doUpdate(Agent&, Element&) override;
    };
};

#endif /* GOMOKU_PLAYER_HPP */
