/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_PLAYER_HPP
#define GOMOKU_PLAYER_HPP

#include <memory>
#include "base/Component.hpp"
#include "Command.hpp"
#include "mbe/Engine.hpp"

class Player {
    class Graphics;
 public:
    class InputHandler;
    using Board = go::State;
    using Input = go::Position;
    using Move = base::Command<Board>;
    using InputComponent = base::Component<Board, Input, Move>;

    Player(InputComponent* const);

    void setTeam(go::Team);
    go::Team getTeam() const;
    Move makeMove(Board&, Input&);    

 private:
    go::Team team = go::Team::BLACK;
    std::unique_ptr<InputComponent> inputPtr;
    InputComponent& input;

};

class Player::InputHandler : public InputComponent {
 private:
    Product doUpdate(Agent&, Element&) override;
};

#endif /* GOMOKU_PLAYER_HPP */
