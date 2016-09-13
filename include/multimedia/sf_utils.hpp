/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */
#ifndef SF_UTILS_HPP
#define SF_UTILS_HPP

namespace sf_utils {
    inline void set_color(sf::Text& text, const sf::Color& color) {
        #if SFML_VERSION_MINOR == 4
            text.setFillColor(color);
        #else
            text.setColor(color);
        #endif
    }
}

#endif /* SF_UTILS_HPP */
