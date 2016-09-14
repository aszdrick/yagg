/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_PLAYER_HPP
#define GOMOKU_PLAYER_HPP

#include <memory>
#include "base/Component.hpp"
#include "base/Player.hpp"
#include "Command.hpp"
#include "mbe/Engine.hpp"

class Player : public base::Player<go::State, std::list<go::Position>> {
    class Graphics;
 public:
    class InputHandler;
    using InputComponent = base::Component<Board, Input, Move>;

    Player(InputComponent* const);

    void setTeam(go::Team);
    go::Team getTeam() const;

 private:
    go::Team team = go::Team::BLACK;
    std::unique_ptr<InputComponent> inputPtr;
    InputComponent& input;

    Move onProcessInput(Board&, Input&) override;    
};

class Player::InputHandler : public InputComponent {
 private:
    Product doUpdate(Agent&, Element&) override;
};

#endif /* GOMOKU_PLAYER_HPP */
