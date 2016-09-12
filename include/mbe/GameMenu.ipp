/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template<typename G>
const sf::Color mbe::GameMenu<G>::SELECTED_COLOR(225, 225, 225);
template<typename G>
const sf::Color mbe::GameMenu<G>::UNSELECTED_COLOR(128, 128, 128);

template<typename G>
mbe::GameMenu<G>::GameMenu(G& gameParam)
 : selected(0), game(gameParam) {
    font.loadFromFile("res/ARCADECLASSIC.TTF");

    options[0].name = sf::Text("New Game", font);
    options[0].name.setCharacterSize(60);
    options[0].color = SELECTED_COLOR;
    options[0].action = [this]() { return game.get().newGame(); };

    options[1].name = sf::Text("Options", font);
    options[1].name.setCharacterSize(60);
    options[1].color = UNSELECTED_COLOR;
    options[1].action = [this]() { return game.get().optionsMenu(); };
    
    options[2].name = sf::Text("Quit", font);
    options[2].name.setCharacterSize(60);
    options[2].color = UNSELECTED_COLOR;
    options[2].action = [this]() { return game.get().quit(); };

}

template<typename G>
void mbe::GameMenu<G>::onUpdateRenderer(Renderer& renderer) {
    auto size = renderer.getSize();
    double width = size.x;
    double height = size.y;
    double padding = OPTIONS_Y_PADDING * height;
    double fixedX = width/2;
    double currentY = height/2 - padding;

    renderer.clear(sf::Color::Black);

    for (auto option : options) {
        auto rect = option.name.getLocalBounds();
        double xOffset = rect.width/2;
        double yOffset = rect.height;

        option.name.setColor(option.color);
        option.name.setPosition(sf::Vector2f(fixedX - xOffset, currentY - yOffset));
        currentY += padding;

        renderer.draw(option.name);
    }
}

template<typename G>
typename mbe::GameMenu<G>::Response
mbe::GameMenu<G>::onProcessInput(Input& eventsList) {
     while (!eventsList.empty()) {
        auto event = eventsList.front();
        eventsList.pop_front();

        switch(event.type) {
            case sf::Event::KeyPressed:
                return keyPressed(event);
            default:;
        }
    }

    return {Response::Type::SELF, 0, nullptr};
}

template<typename G>
typename mbe::GameMenu<G>::Response
mbe::GameMenu<G>::keyPressed(const sf::Event& event) {
    switch (event.key.code) {
        case sf::Keyboard::Return:
            return {Response::Type::STORE, 0, options[selected].action()};
        case sf::Keyboard::Up:
            changeOption((3 + selected - 1) % 3);
            break;
        case sf::Keyboard::Down:
            changeOption((selected + 1) % 3);
            break;
        default:;
    }
    return {Response::Type::SELF, 0, nullptr};
}

template<typename G>
void mbe::GameMenu<G>::changeOption(short change) {
    options[selected].color = UNSELECTED_COLOR;
    selected = change;
    options[selected].color = SELECTED_COLOR;
}