

const int menuItemsNumber = 3;
int selectedItem = 0;

void returnPressedHandle()
{
    switch (selectedItem)
    {
    case 0:
        gameState = input_name;
        break;
    case 1:
        break;
    case 2:
        gameState = quit;
        break;
    }
}

void moveUp(sf::Text text[])
{
    if (selectedItem - 1 >= 0)
    {
        text[selectedItem].setFillColor(sf::Color::White);
        text[selectedItem].setStyle(sf::Text::Regular);
        selectedItem--;
        text[selectedItem].setFillColor(sf::Color::Red);
        text[selectedItem].setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
}

void moveDown(sf::Text text[])
{
    if (selectedItem + 1 < menuItemsNumber)
    {
        text[selectedItem].setFillColor(sf::Color::White);
        text[selectedItem].setStyle(sf::Text::Regular);
        selectedItem++;
        text[selectedItem].setFillColor(sf::Color::Red);
        text[selectedItem].setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
}

void drawMenuItems(sf::RenderWindow &window, sf::Text text[])
{
    for (int i = 0; i < menuItemsNumber; i++)
    {
        window.draw(text[i]);
    }
}

void menu(sf::RenderWindow &window)
{
    const float windowWidth = window.getSize().x;
    const float windowHeight = window.getSize().y;
    sf::Font font;
    font.loadFromFile("00/arial.ttf");
    sf::Text text[menuItemsNumber];
    std::string menuItems[menuItemsNumber] = {"Play", "HighScores", "Exit"};

    for (int i = 0; i < menuItemsNumber; i++)
    {
        text[i].setFillColor(sf::Color::White);
        text[i].setFont(font);
        text[i].setString(menuItems[i]);
        text[i].setPosition({x : windowWidth / 3, y : windowHeight / (menuItemsNumber + 1) * (i + 1)});
    }
    text[0].setFillColor(sf::Color::Red);
    text[0].setStyle(sf::Text::Bold | sf::Text::Underlined);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    moveUp(text);
                    break;

                case sf::Keyboard::Down:
                    moveDown(text);
                    break;

                case sf::Keyboard::Return:
                    returnPressedHandle();
                    return;
                }
                break;
            case sf::Event::Closed:
                gameState = quit;
                return;
            }
        }
        window.clear();
        drawMenuItems(window, text);
        window.display();
    }
}