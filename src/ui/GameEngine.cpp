/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include "ui/GameEngine.hpp"

GameEngine::GameEngine(double width, double height, const std::string& title)
 : WIDTH(width),
   HEIGHT(height),
   TITLE(title),
   window(sf::VideoMode(WIDTH, HEIGHT),
          TITLE,
          sf::Style::Default,
          desiredContextSettings()) {
    window.setVerticalSyncEnabled(true);
}

void GameEngine::run() {
    window.setActive(true);

    while (window.isOpen()) {    
       
        manageEvents();

        window.display();
    }
}

bool GameEngine::isRunning() {
    return window.isOpen();
}

void GameEngine::manageEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default: break;
        }
    }
}

sf::ContextSettings GameEngine::desiredContextSettings() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    return settings;
}