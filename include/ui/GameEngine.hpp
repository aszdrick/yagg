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

template<typename Game, typename UI>
class GameEngine {
 public:
    GameEngine(Game&, UI&, double = 800, double = 600,
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
    Game& game;
    UI& interface;

    void render() const;
    void manageEvents();
    void onMousePressed(const sf::Event&);
    void onMouseReleased(const sf::Event&);
    static sf::ContextSettings desiredContextSettings();
};

#include "GameEngine.ipp"

#endif /* GAME_ENGINE_HPP */
