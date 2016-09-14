/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef MBE_ENGINE_HPP
#define MBE_ENGINE_HPP

// SFML Libraries
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// Standard Libraries
#include <string>

// Custom classes
#include "base/Game.hpp"

namespace mbe {
    using Game = base::Game<std::list<sf::Event>, sf::RenderWindow>;
    
    class Engine {
     public:
        static const double MAX_WIDTH;
        static const double MAX_HEIGHT;
        static const double MAX_DEPTH;

        Engine(Game* const,
               const std::string& = "MBE",
               double = MAX_WIDTH,
               double = MAX_HEIGHT);

        void run();
        bool isRunning() const;

     private:
        const std::string TITLE;
        const double DEFAULT_WIDTH;
        const double DEFAULT_HEIGHT;
        static const sf::VideoMode BEST_VIDEO_MODE;
        std::unique_ptr<Game::Renderer> windowPtr;
        Game::Renderer& window;
        std::unique_ptr<Game> gamePtr;
        Game& game;
        bool fullscreen = false;

        void processEvents();
        void resize(double, double);
        void switchScreenMode();
        static sf::ContextSettings desiredContextSettings();
    };
}

#endif /* MBE_ENGINE_HPP */
