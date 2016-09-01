/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#include "mbe/Engine.hpp"

namespace mbe {    
    Engine::Engine(Game* const game, double width, double height,
        const std::string& title)
     : WIDTH(width), HEIGHT(height), TITLE(title),
       windowPtr(new Game::Renderer(
        sf::VideoMode(WIDTH, HEIGHT),
        TITLE, sf::Style::Default,
        desiredContextSettings())),
       window(*windowPtr),
       gamePtr(game), game(*gamePtr) {
        
        window.setVerticalSyncEnabled(true);
    }

    void Engine::run() {
        window.setActive(true);

        while (window.isOpen()) {    
           
            manageEvents();

            window.clear(sf::Color::Black);

            // Hu3
            game.updateLogic();
            game.updateRenderer(window);

            window.display();
        }
    }

    bool Engine::isRunning() const {
        return window.isOpen();
    }

    void Engine::manageEvents() {
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

    void Engine::resize(double width, double height) {
        window.setSize(sf::Vector2u(width, height));
    }

    // void Engine::onMousePressed(const sf::Event& event) {
    //     auto x = event.mouseButton.x;
    //     auto y = event.mouseButton.y;
    //     auto action = interface.handleMousePressed(x, y);
    //     (void) action;
    // }

    // void Engine::onMouseReleased(const sf::Event& event) {
    //     auto x = event.mouseButton.x;
    //     auto y = event.mouseButton.y;
    //     auto action = interface.handleMouseReleased(x, y);
    //     (void) action;
    // }

    sf::ContextSettings Engine::desiredContextSettings() {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 2;
        // settings.majorVersion = 3;
        // settings.minorVersion = 2;
        return settings;
    }
}
