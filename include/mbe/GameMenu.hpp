/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef MBE_GAME_MENU_HPP
#define MBE_GAME_MENU_HPP

#include "Engine.hpp"

namespace mbe {
    template<typename G>
    class GameMenu : public Game::State {
     public:
        static constexpr double OPTIONS_Y_PADDING = 0.15;
        static constexpr double OPTIONS_X_PADDING = 0.15;
        static const sf::Color SELECTED_COLOR;
        static const sf::Color UNSELECTED_COLOR;
        struct Option;

        GameMenu(G&);

     private:
        double width;
        double height;
        unsigned short selected;
        std::array<Option, 3> options;
        std::reference_wrapper<G> game;
        sf::Font font;

        void onUpdateRenderer(Renderer&) override;
        Response onProcessInput(Input&) override;
        void onSetVideoMode(Renderer&, double, double, double) override;

        void keyPressed(const sf::Event&);
        void changeOption(short);
    };

    template<typename G>
    struct GameMenu<G>::Option {
        sf::Text name;
        sf::Color color;
        std::function<void()> action;
    };
}

#include "GameMenu.ipp"

#endif /* MBE_GAME_MENU_HPP */