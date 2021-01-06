const int menuItemsNumber = 3;
int selectedMenuItem;
sf::Text menuItems[menuItemsNumber];

void returnPressedHandle()
{
    switch (selectedMenuItem)
    {
    case 0:
        gameState = input_name;
        break;
    case 1:
        gameState = high_scores;
        break;
    case 2:
        gameState = quit;
        break;
    }
}

void moveUp(sf::Text menuItems[])
{
    if (selectedMenuItem - 1 >= 0)
    {
        menuItems[selectedMenuItem].setFillColor(sf::Color::Yellow);
        menuItems[selectedMenuItem].setStyle(sf::Text::Regular);
        selectedMenuItem--;
        menuItems[selectedMenuItem].setFillColor(sf::Color::Red);

        menuItems[selectedMenuItem].setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
}

void moveDown(sf::Text menuItems[])
{
    if (selectedMenuItem + 1 < menuItemsNumber)
    {
        menuItems[selectedMenuItem].setFillColor(sf::Color::Yellow);
        menuItems[selectedMenuItem].setStyle(sf::Text::Regular);
        selectedMenuItem++;
        menuItems[selectedMenuItem].setFillColor(sf::Color::Red);
        menuItems[selectedMenuItem].setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
}

void drawMenuItems(sf::RenderWindow &window, sf::Text menuItems[])
{
    for (int i = 0; i < menuItemsNumber; i++)
    {
        window.draw(menuItems[i]);
    }
}

void createMenu(const float windowWidth, const float windowHeight)
{
    std::string menuItemsStrings[menuItemsNumber] = {"Play", "HighScores", "Exit"};
    for (int i = 0; i < menuItemsNumber; i++)
    {
        menuItems[i].setFillColor(sf::Color::Yellow);
        menuItems[i].setFont(getFont());
        menuItems[i].setString(menuItemsStrings[i]);
        menuItems[i].setPosition({x : windowWidth / 3, y : windowHeight / (menuItemsNumber + 1) * (i + 1)});
    }
    menuItems[0].setFillColor(sf::Color::Red);
    menuItems[0].setStyle(sf::Text::Bold | sf::Text::Underlined);
}

void menuPollEvents(sf::RenderWindow &window)
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
                break;
            }
            break;
        case sf::Event::Closed:
            gameState = quit;
            break;
        }
    }
}

void menu(sf::RenderWindow &window)
{
    const float windowWidth = static_cast<float>(WINDOW_WIDTH);
    const float windowHeight = static_cast<float>(WINDOW_HEIGHT);
    selectedMenuItem = 0;
    sf::Sprite background;
    background.setTexture(getBackgroundMenuTexture());
    createMenu(windowWidth, windowHeight);

    while (window.isOpen())
    {
        menuPollEvents(window);
        window.clear();
        window.draw(background);
        drawMenuItems(window, menuItems);
        window.display();
        if (
            gameState == input_name ||
            gameState == high_scores ||
            gameState == quit)
            return;
    }
}