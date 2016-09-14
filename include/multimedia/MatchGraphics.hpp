/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef GOMOKU_MATCH_GRAPHICS_HPP
#define GOMOKU_MATCH_GRAPHICS_HPP

#include "gomoku/Match.hpp"

class Match::Graphics : public GraphicalComponent {
 public:
    Graphics();
 private:
    sf::Font font;
    void doUpdate(Agent&, Element&) override;
    void drawBoard(Element&) const;
    void drawWinner(Agent&, Element&) const;
    void drawStatus(Agent&, Element&) const; 
    void drawStones(Agent&, Element&) const;
    void highlight(Agent&, Element&) const;
    sf::Text prepareText(const std::string&, unsigned) const;
};

#endif /* GOMOKU_MATCH_GRAPHICS_HPP */