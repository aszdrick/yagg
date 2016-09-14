/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#include <cmath>
#include "mbe/Engine.hpp"
#include "extra/macros.hpp"

const sf::VideoMode mbe::Engine::BEST_VIDEO_MODE = sf::VideoMode::getDesktopMode();

const double mbe::Engine::MAX_WIDTH = BEST_VIDEO_MODE.width;
const double mbe::Engine::MAX_HEIGHT = BEST_VIDEO_MODE.height;
const double mbe::Engine::MAX_DEPTH = BEST_VIDEO_MODE.bitsPerPixel;

mbe::Engine::Engine(Game* const gameParam,
                    const std::string& title,
                    double width,
                    double height)
 : TITLE(title),
   DEFAULT_WIDTH(width),
   DEFAULT_HEIGHT(height),
   windowPtr(new Game::Renderer(
        sf::VideoMode(width, height, MAX_DEPTH),
        TITLE, sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close,
        desiredContextSettings())),
   window(*windowPtr),
   gamePtr(gameParam),game(*gamePtr) {
    
    window.setVerticalSyncEnabled(true);

    game.setVideoMode(window, width, height, MAX_DEPTH);
}

void mbe::Engine::run() {
    window.setActive(true);

    while (window.isOpen()) {               
        // Process events & update logic
        processEvents();
        // Some day this call will become periodic
        // for some "usefull" stuff like in-game clock
        game.periodicUpdate();

        if (game.switchScreenModeRequested()) {
            switchScreenMode();
        }

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
    // double aspectRatio = currWidth / currHeight;
    // double deltaWidth = std::abs(width - currWidth);
    // double deltaHeight = std::abs(height - currHeight);
    // if (deltaWidth > deltaHeight) {
    //     currWidth = width;
    //     currHeight = currWidth / aspectRatio;
    // } else {
    //     currHeight = height;
    //     currWidth = currHeight * aspectRatio;
    // }
    // window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
    game.setVideoMode(window, DEFAULT_WIDTH, DEFAULT_HEIGHT, MAX_DEPTH);
}

void mbe::Engine::switchScreenMode() {
    if (fullscreen) {
        window.create(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT, MAX_DEPTH),
        TITLE, sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close,
        desiredContextSettings());
        fullscreen = false;
    } else {
        auto view = window.getView();
        window.create(BEST_VIDEO_MODE, TITLE, sf::Style::Fullscreen,
            desiredContextSettings());
        window.setView(view);
        fullscreen = true;
    }
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
