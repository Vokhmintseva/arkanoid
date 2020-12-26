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

void moveUp(sf::Text menuItems[])
{
    if (selectedItem - 1 >= 0)
    {
        menuItems[selectedItem].setFillColor(sf::Color::White);
        menuItems[selectedItem].setStyle(sf::Text::Regular);
        selectedItem--;
        menuItems[selectedItem].setFillColor(sf::Color::Red);
        menuItems[selectedItem].setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
}

void moveDown(sf::Text menuItems[])
{
    if (selectedItem + 1 < menuItemsNumber)
    {
        menuItems[selectedItem].setFillColor(sf::Color::White);
        menuItems[selectedItem].setStyle(sf::Text::Regular);
        selectedItem++;
        menuItems[selectedItem].setFillColor(sf::Color::Red);
        menuItems[selectedItem].setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
}

void drawMenuItems(sf::RenderWindow &window, sf::Text menuItems[])
{
    for (int i = 0; i < menuItemsNumber; i++)
    {
        window.draw(menuItems[i]);
    }
}

void menu(sf::RenderWindow &window)
{
    const float windowWidth = window.getSize().x;
    const float windowHeight = window.getSize().y;
    sf::Font font;
    font.loadFromFile("00/arial.ttf");
    sf::Text menuItems[menuItemsNumber];
    std::string menuItemsStrings[menuItemsNumber] = {"Play", "HighScores", "Exit"};

    for (int i = 0; i < menuItemsNumber; i++)
    {
        menuItems[i].setFillColor(sf::Color::White);
        menuItems[i].setFont(font);
        menuItems[i].setString(menuItemsStrings[i]);
        menuItems[i].setPosition({x : windowWidth / 3, y : windowHeight / (menuItemsNumber + 1) * (i + 1)});
    }
    menuItems[0].setFillColor(sf::Color::Red);
    menuItems[0].setStyle(sf::Text::Bold | sf::Text::Underlined);

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
                    moveUp(menuItems);
                    break;

                case sf::Keyboard::Down:
                    moveDown(menuItems);
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
        drawMenuItems(window, menuItems);
        window.display();
    }
}