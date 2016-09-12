/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include "gomoku/Traits.hpp"
#include "multimedia/MatchGraphics.hpp"
#include "extra/macros.hpp"

Gomoku::Match::Graphics::Graphics() {
    font.loadFromFile("res/ARCADECLASSIC.TTF");
}

void Gomoku::Match::Graphics::doUpdate(Agent& match, Element& window) {
    drawBoard(window);
    if (match.isOver()) {
        drawGameOverScreen(match, window);
    } else {
        highlight(match, window);    
    }
    drawStones(match, window);
}

void Gomoku::Match::Graphics::drawBoard(Element& window) const {
    static std::vector<sf::Vertex> lines;
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    if (lines.empty()) {
        auto squareSize = MatchTraits::SQUARE_SIZE;
        auto boardStart = MatchTraits::BORDER_WIDTH;
        auto boardEnd = boardStart + (boardDimension - 1) * squareSize;
        auto offset = [=](unsigned i) {
            return boardStart + i * squareSize;
        };
        
        for (unsigned i = 0; i < boardDimension; i++) {
            lines.emplace_back(sf::Vector2f(boardStart, offset(i)));
            lines.emplace_back(sf::Vector2f(boardEnd, offset(i)));
        }

        for (unsigned i = 0; i < boardDimension; i++) {
            lines.emplace_back(sf::Vector2f(offset(i), boardStart));
            lines.emplace_back(sf::Vector2f(offset(i), boardEnd));
        }
    }

    window.clear(sf::Color::Black);
    window.draw(&lines.front(), 4 * boardDimension, sf::Lines);
}

void Gomoku::Match::Graphics::drawStones(Agent& match, Element& window) const {
    match.state.iterate([&window](auto& stone) {
        auto shape = sf::CircleShape(MatchTraits::STONE_RADIUS, 50);
        auto squareSize = MatchTraits::SQUARE_SIZE;
        shape.setPosition(sf::Vector2f(
            squareSize + stone.position.column * squareSize 
            - MatchTraits::STONE_RADIUS,
            squareSize + stone.position.row * squareSize 
            - MatchTraits::STONE_RADIUS));

        auto white = GomokuTraits::WHITE_COLOR;
        auto black = GomokuTraits::BLACK_COLOR;
        auto w_border = GomokuTraits::WHITE_OUTLINE_COLOR;
        auto b_border = GomokuTraits::BLACK_OUTLINE_COLOR;

        shape.setOutlineThickness(MatchTraits::STONE_BORDER_WIDTH);
        
        shape.setOutlineColor(stone.team == go::Team::WHITE ? w_border : b_border);

        shape.setFillColor(stone.team == go::Team::WHITE ? white : black);
        window.draw(shape);
    });
}

void Gomoku::Match::Graphics::drawGameOverScreen(Agent& match, Element& window) const {
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    auto squareSize = MatchTraits::SQUARE_SIZE;
    auto boardStart = MatchTraits::BORDER_WIDTH;
    auto boardEnd = boardStart + (boardDimension - 1) * squareSize;

    sf::Text text(go::to_string(match.winnerTeam()) + " wins!", font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(boardEnd + MatchTraits::TEXT_PADDING, 100));

    window.draw(text);
}

void Gomoku::Match::Graphics::highlight(Agent& match, Element& window) const {
// struct BoardAnalyzer::Sequence {
//     std::vector<go::Stone*> stones;
//     std::pair<bool, bool> freeEnds = {false, false};
// };
    auto drawHighlightedSpot = [&](const go::Position& position) {
        int signedDimension = static_cast<int>(GomokuTraits::BOARD_DIMENSION);
        if (position.row < 0 || position.column < 0
            || position.row >= signedDimension
            || position.column >= signedDimension) {
            return;
        }
        auto shape = sf::CircleShape(MatchTraits::STONE_RADIUS, 50);
        auto squareSize = MatchTraits::SQUARE_SIZE;
        shape.setPosition(sf::Vector2f(
            squareSize + position.column * squareSize 
            - MatchTraits::STONE_RADIUS,
            squareSize + position.row * squareSize 
            - MatchTraits::STONE_RADIUS));

        shape.setOutlineThickness(MatchTraits::STONE_BORDER_WIDTH);
        shape.setOutlineColor(GomokuTraits::HIGHLIGHT_OUTLINE_COLOR);
        shape.setFillColor(GomokuTraits::HIGHLIGHT_COLOR);
        window.draw(shape);
    };

    match.state.quadrupletIteration([&](auto& sequence) {
        go::Stone& first = *sequence.stones.front();
        go::Stone& last = *sequence.stones.back();
        unsigned distance = go::Position::distance(first.position, last.position);
        if (distance == 4) {
            go::Position hole;
            go::Position* prev = &first.position;
            for (unsigned i = 1; i < sequence.stones.size(); i++) {
                go::Stone& stone = *sequence.stones[i];
                go::Position& position = stone.position;
                if (go::Position::distance(position, *prev) > 1) {
                    hole = (position + *prev) / 2;
                    break;
                }
                prev = &position;
            }
            drawHighlightedSpot(hole);
        } else {
            if (sequence.freeEnds.first) {
                go::Position highlighted = first.position - sequence.delta;
                drawHighlightedSpot(highlighted);
            }

            if (sequence.freeEnds.second) {
                go::Position highlighted = last.position + sequence.delta;
                drawHighlightedSpot(highlighted);
            }            
        }
    });
}
