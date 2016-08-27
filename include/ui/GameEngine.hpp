/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <thread>

class GameEngine {
 public:
    GameEngine(double = 800, double = 600,
        const std::string& = "SFML Window");

    void run();
    bool isRunning();

 private:
    const double WIDTH;
    const double HEIGHT;
    const std::string TITLE;
    sf::RenderWindow window;

    void manageEvents();
    sf::ContextSettings desiredContextSettings();
};

#endif /* GAME_ENGINE_HPP */
