/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef MBE_MENU_HPP
#define MBE_MENU_HPP

#include "Engine.hpp"
#include "gomoku/Traits.hpp"

namespace mbe {
    template<typename G, unsigned short S>
    class Menu : public Game::State {
     public:
        static constexpr double OPTIONS_PADDING = 0.15;
        static const sf::Color SELECTED_COLOR;
        static const sf::Color UNSELECTED_COLOR;
        struct Option;

        Menu(G&, const std::array<Option, S>&,
             const std::string& = "res/ARCADECLASSIC.TTF");

     private:
        double width = GomokuTraits::WINDOW_WIDTH;
        double height = GomokuTraits::WINDOW_HEIGHT;
        unsigned short selected;
        std::array<Option, S> options;
        std::reference_wrapper<G> game;
        sf::Font font;

        void onUpdateRenderer(Renderer&) override;
        Response onProcessInput(InputProcessor&, Input&) override;
        void onResize(double, double) override;

        void keyPressed(const sf::Event&);
        void changeOption(short);
    };

    template<typename G, unsigned short S>
    struct Menu<G,S>::Option {
        sf::Text name;
        sf::Color color;
        std::function<void(G&)> action;
    };
}

#include "Menu.ipp"

#endif /* MBE_MENU_HPP */