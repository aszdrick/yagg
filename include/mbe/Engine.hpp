/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

// SFML Libraries
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// Standard Libraries
#include <string>

// Custom classes
#include "base/Game.hpp"

namespace mbe {
    using Game = Game<sf::RenderWindow, std::list<sf::Event>>;
    
    class Engine {
     public:
        Engine(Game&, double = 800, double = 600,
            const std::string& = "SFML Window");

        void run();
        void resize(double, double);
        bool isRunning() const;

     private:
        const double WIDTH;
        const double HEIGHT;
        const std::string TITLE;
        std::unique_ptr<Game::Renderer> windowPtr;
        Game::Renderer& window;
        Game& game;

        void manageEvents();
        static sf::ContextSettings desiredContextSettings();
    };
}

#endif /* GAME_ENGINE_HPP */