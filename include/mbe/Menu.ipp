/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#include "extra/macros.hpp"
#include "multimedia/sf_utils.hpp"

template<typename G, unsigned short S>
const sf::Color mbe::Menu<G,S>::SELECTED_COLOR(225, 225, 225);
template<typename G, unsigned short S>
const sf::Color mbe::Menu<G,S>::UNSELECTED_COLOR(128, 128, 128);

template<typename G, unsigned short S>
mbe::Menu<G,S>::Menu(G& game, const std::array<Option, S>& opt,
                     const std::string& fontName)
 : selected(0), options{opt}, game(game) {
    font.loadFromFile(fontName);

    for (auto& option : options) {
        option.color = UNSELECTED_COLOR;
        option.name.setFont(font);
        option.name.setCharacterSize(60);
    }

    options.at(0).color = SELECTED_COLOR;
}

template<typename G, unsigned short S>
void mbe::Menu<G,S>::onUpdateRenderer(Renderer& renderer) {
    double padding = OPTIONS_PADDING * height;
    double fixedX = width/2;
    double currentY = height/2 - (padding/2 * (S - 1));

    renderer.clear(sf::Color::Black);

    for (auto& option : options) {
        auto rect = option.name.getLocalBounds();
        double xOffset = rect.width/2;
        double yOffset = rect.height;

        sf_utils::set_color(option.name, option.color);
        option.name.setPosition(sf::Vector2f(fixedX - xOffset, currentY - yOffset));
        currentY += padding;

        renderer.draw(option.name);
    }
}

template<typename G, unsigned short S>
typename mbe::Menu<G,S>::Response mbe::Menu<G,S>::onProcessInput(Input& events) {
     while (!events.empty()) {
        auto event = events.front();
        events.pop_front();

        switch(event.type) {
            case sf::Event::KeyPressed:
                keyPressed(event);
                break;
            default:;
        }
    }

    return {Response::Type::SELF, 0, nullptr};
}

template<typename G, unsigned short S>
void mbe::Menu<G,S>::onResize(double w, double h) {
    static bool defined = false;
    if (!defined) {
        width = w;
        height = h;
        defined = true;
    }
}

template<typename G, unsigned short S>
void mbe::Menu<G,S>::keyPressed(const sf::Event& event) {
    switch (event.key.code) {
        case sf::Keyboard::Return:
            options[selected].action(game);
            break;
        case sf::Keyboard::Up:
            changeOption((S + selected - 1) % S);
            break;
        case sf::Keyboard::Down:
            changeOption((selected + 1) % S);
            break;
        default:;
    }
}

template<typename G, unsigned short S>
void mbe::Menu<G,S>::changeOption(short change) {
    options[selected].color = UNSELECTED_COLOR;
    selected = change;
    options[selected].color = SELECTED_COLOR;
}