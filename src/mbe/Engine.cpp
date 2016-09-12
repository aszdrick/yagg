/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#include "mbe/Engine.hpp"

const sf::VideoMode mbe::Engine::BEST_VIDEO_MODE = 
    sf::VideoMode::getFullscreenModes()[0];

const double mbe::Engine::MAX_WIDTH = BEST_VIDEO_MODE.width;
const double mbe::Engine::MAX_HEIGHT = BEST_VIDEO_MODE.height;
const double mbe::Engine::MAX_DEPTH = BEST_VIDEO_MODE.bitsPerPixel;

mbe::Engine::Engine(Game* const gameParam,
                    const std::string& title,
                    double width,
                    double height)
 : TITLE(title),
   windowPtr(new Game::Renderer(
        sf::VideoMode(width, height, BEST_VIDEO_MODE.bitsPerPixel),
        TITLE, sf::Style::Default,
        desiredContextSettings())),
   window(*windowPtr),
   gamePtr(gameParam),game(*gamePtr) {
    
    window.setVerticalSyncEnabled(true);

    game.setVideoMode(width, height, MAX_DEPTH);
}

void mbe::Engine::run() {
    window.setActive(true);

    while (window.isOpen()) {               
        // Process events & update logic
        processEvents();
        // Some day this call will become periodic
        // for some "usefull" stuff like in-game clock
        game.periodicUpdate();

        game.updateRenderer(window);

        window.display();
    }
}

bool mbe::Engine::isRunning() const {
    return window.isOpen();
}

void mbe::Engine::processEvents() {
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
    
    game.processInput(list);

    if (game.closed()) {
        window.close();
    }
}

void mbe::Engine::resize(double width, double height) {
    window.setSize(sf::Vector2u(width, height));
    game.setVideoMode(width, height, MAX_DEPTH);
}

sf::ContextSettings mbe::Engine::desiredContextSettings() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    // settings.majorVersion = 3;
    // settings.minorVersion = 2;
    return settings;
}
