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
            // Process events & update logic
            processEvents();
            // Some day this call will become periodic
            // for some "usefull" stuff like in-game clock
            game.syncUpdate();

            game.updateRenderer(window);

            window.display();
        }
    }

    bool Engine::isRunning() const {
        return window.isOpen();
    }

    void Engine::processEvents() {
        std::list<sf::Event> list;
        sf::Event event;

        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    if (game.close()) {
                        window.close();
                    }
                    break;
                case sf::Event::Resized:
                    resize(event.size.width, event.size.height);
                    break;
                default:
                    list.push_back(event);
                    break;
            }
        }
        
        game.processEvents(list);

        if (game.closed()) {
            window.close();
        }
    }

    void Engine::resize(double width, double height) {
        window.setSize(sf::Vector2u(width, height));
    }

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
