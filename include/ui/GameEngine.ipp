/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include "ui/GameEngine.hpp"

template<typename G, typename I>
GameEngine<G,I>::GameEngine(G& game, I& interface,
    double width, double height, const std::string& title)
 : WIDTH(width),
   HEIGHT(height),
   TITLE(title),
   windowPtr(new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT),
          TITLE,
          sf::Style::Default,
          desiredContextSettings())),
   window(*windowPtr),
   game(game),
   interface(interface) {
    window.setVerticalSyncEnabled(true);
}

template<typename G, typename I>
void GameEngine<G,I>::run() {
    window.setActive(true);

    while (window.isOpen()) {    
       
        manageEvents();

        window.clear(sf::Color::Black);

        render();

        window.display();
    }
}

template<typename G, typename I>
bool GameEngine<G,I>::isRunning() const {
    return window.isOpen();
}

template<typename G, typename I>
void GameEngine<G,I>::render() const {
    interface.draw(window);
}

template<typename G, typename I>
void GameEngine<G,I>::manageEvents() {
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
                onMousePressed(event);
                break;
            case sf::Event::MouseButtonReleased:
                onMouseReleased(event);
                break;
            default: break;
        }
    }
}

template<typename G, typename I>
void GameEngine<G,I>::resize(double width, double height) {
    window.setSize(sf::Vector2u(width, height));
}

template<typename G, typename I>
void GameEngine<G,I>::onMousePressed(const sf::Event& event) {
    auto action = interface.handleMousePressed(event.mouseButton.x, event.mouseButton.y);
    (void) action;
}

template<typename G, typename I>
void GameEngine<G,I>::onMouseReleased(const sf::Event& event) {
    auto action = interface.handleMouseReleased(event.mouseButton.x, event.mouseButton.y);
    (void) action;
}

template<typename G, typename I>
sf::ContextSettings GameEngine<G,I>::desiredContextSettings() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    // settings.majorVersion = 3;
    // settings.minorVersion = 2;
    return settings;
}
