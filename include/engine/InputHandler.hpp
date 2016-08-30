/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "base/InputComponent.hpp"
#include "base/Player.hpp"
#include <SFML/Graphics.hpp>

template<>
class InputComponent<Player<sf::RenderWindow>, sf::RenderWindow> {
 public:
    void update(const Player<sf::RenderWindow>&, sf::RenderWindow&);
};

using InputHandler = InputComponent<Player<sf::RenderWindow>, sf::RenderWindow>;

#endif /* INPUT_HANDLER_HPP */