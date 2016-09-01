/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GOMOKU_PLAYER_HPP
#define GOMOKU_PLAYER_HPP

#include <memory>
#include "base/Component.hpp"
#include "Game.hpp"

namespace gomoku {
    using Point = std::pair<unsigned, unsigned>;

    struct Move {
        Point coords;
        bool valid;
    };

    class Player : public Game::Player {
     public:
        using Graphics = Component<Player, Renderer>;
        using InputHandler = Component<Point, Input>; 

        Player(Graphics* const, InputHandler* const);
        Move pendingMove() const;

     private:
        std::unique_ptr<Graphics> graphicsPtr;
        std::unique_ptr<InputHandler> inputPtr;
        Graphics& graphics;
        InputHandler& input;
        Move lastMove;

        void updateGraphics(Renderer&) override;
        void processInput(Input&) override;
    };
    
}

#endif /* GOMOKU_PLAYER_HPP */
