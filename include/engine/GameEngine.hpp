/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include "base/Game.hpp"

using SFMLGame = Game<sf::RenderWindow>;

class GameEngine {
 public:
    GameEngine(SFMLGame&, double = 800, double = 600,
        const std::string& = "SFML Window");

    void run();
    void resize(double, double);
    bool isRunning() const;

 private:
    const double WIDTH;
    const double HEIGHT;
    const std::string TITLE;
    std::unique_ptr<sf::RenderWindow> windowPtr;
    sf::RenderWindow& window;
    SFMLGame& game;

    void manageEvents();
    static sf::ContextSettings desiredContextSettings();
};

#endif /* GAME_ENGINE_HPP */
