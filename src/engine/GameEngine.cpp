/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include "engine/GameEngine.hpp"

GameEngine::GameEngine(SFMLGame& game, double width, double height,
    const std::string& title)
 : WIDTH(width),
   HEIGHT(height),
   TITLE(title),
   windowPtr(new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT),
          TITLE,
          sf::Style::Default,
          desiredContextSettings())),
   window(*windowPtr),
   game(game) {
    window.setVerticalSyncEnabled(true);
}

void GameEngine::run() {
    window.setActive(true);

    while (window.isOpen()) {    
       
        manageEvents();

        window.clear(sf::Color::Black);

        // Hu3
        game.update(window);

        window.display();
    }
}

bool GameEngine::isRunning() const {
    return window.isOpen();
}

void GameEngine::manageEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                resize(event.size.width, event.size.height);
                break;
            case sf::Event::MouseButtonPressed:
                //onMousePressed(event);
                break;
            case sf::Event::MouseButtonReleased:
                //onMouseReleased(event);
                break;
            default: break;
        }
    }
}

void GameEngine::resize(double width, double height) {
    window.setSize(sf::Vector2u(width, height));
}

// void GameEngine::onMousePressed(const sf::Event& event) {
//     auto x = event.mouseButton.x;
//     auto y = event.mouseButton.y;
//     auto action = interface.handleMousePressed(x, y);
//     (void) action;
// }

// void GameEngine::onMouseReleased(const sf::Event& event) {
//     auto x = event.mouseButton.x;
//     auto y = event.mouseButton.y;
//     auto action = interface.handleMouseReleased(x, y);
//     (void) action;
// }

sf::ContextSettings GameEngine::desiredContextSettings() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    // settings.majorVersion = 3;
    // settings.minorVersion = 2;
    return settings;
}
