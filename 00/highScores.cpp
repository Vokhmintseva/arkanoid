#include <list>

void highScoresPollEvents(sf::RenderWindow &window, bool &hover, sf::RectangleShape &returnButton)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
            case sf::Keyboard::Return:
                gameState = menu_screen;
                break;
            case sf::Keyboard::Escape:
                gameState = menu_screen;
                break;
            }
            break;
        case sf::Event::MouseMoved:
            sf::IntRect(275, 500, 250, 70).contains(sf::Mouse::getPosition(window)) ? hover = true : hover = false;
            hover ? returnButton.setFillColor(sf::Color(255, 203, 0)) : returnButton.setFillColor(sf::Color(198, 171, 66));
            break;
        case sf::Event::MouseButtonPressed:
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && hover)
                gameState = menu_screen;
            break;
        case sf::Event::Closed:
            gameState = quit;
            break;
        }
    }
}

void readHighScoresFromFile(std::list<Player> &bestScores)
{
    std::ifstream fileout("HighScores.txt"); // окрываем файл для чтения
    if (fileout.is_open())
    {
        std::string line;
        while (getline(fileout, line))
        {
            if (line.empty())
                break;
            size_t i = line.find(' ');
            std::string scoreStr = line.substr(0, i);
            int score = std::stoi(scoreStr);
            std::string name = line.substr(i + 1, line.length());
            bestScores.push_back({score, name});
        }
    }
    fileout.close();
}

void drawHighScores(sf::RenderWindow &window, std::list<Player> &bestScores, sf::Text playerName, sf::Text playerScore)
{
    sf::Vector2f playerNamePos = sf::Vector2f(300, 250);
    sf::Vector2f playerScorePos = sf::Vector2f(450, 250);

    for (Player player : bestScores)
    {
        playerName.setPosition(playerNamePos);
        playerName.setString(player.name);
        window.draw(playerName);

        playerScore.setPosition(playerScorePos);
        playerScore.setString(std::to_string(player.scores));
        window.draw(playerScore);

        playerNamePos.y = playerNamePos.y + 50;
        playerScorePos.y = playerScorePos.y + 50;
    }
}

void showHighScores(sf::RenderWindow &window)
{
    sf::Sprite trophy;
    trophy.setTexture(getTrophyTexture());
    trophy.setPosition(150, 100);

    sf::Sprite legend;
    legend.setTexture(getHighScoresLegendTexture());
    legend.setPosition(300, 80);

    sf::Text playerName;
    playerName.setFillColor(sf::Color(44, 72, 118));
    playerName.setCharacterSize(30);
    playerName.setFont(getFont());

    sf::Text playerScore;
    playerScore.setFillColor(sf::Color::Blue);
    playerScore.setCharacterSize(30);
    playerScore.setFont(getFont());

    sf::RectangleShape returnButton;
    returnButton.setSize(sf::Vector2f(250, 70));
    returnButton.setPosition(275, 500);
    returnButton.setFillColor(sf::Color(198, 171, 66));

    sf::Text returnButtonText;
    returnButtonText.setPosition(280, 515);
    returnButtonText.setFont(getFont());
    returnButtonText.setCharacterSize(30);
    returnButtonText.setFillColor(sf::Color::Black);
    returnButtonText.setString("RETURN TO MENU");

    std::list<Player> bestScores;
    readHighScoresFromFile(bestScores);
    bool hover = false;
    while (window.isOpen())
    {
        highScoresPollEvents(window, hover, returnButton);
        window.clear();
        window.draw(trophy);
        window.draw(legend);
        window.draw(returnButton);
        window.draw(returnButtonText);
        drawHighScores(window, bestScores, playerName, playerScore);
        window.display();
        if (gameState == menu_screen || gameState == quit)
            return;
    }
}