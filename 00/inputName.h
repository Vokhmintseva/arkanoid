#include <string>

void getPlayerName(sf::RenderWindow &window, std::string &plName)
{
    if (plName != "")
    {
        gameState = start_game;
        return;
    }
    bool hover = false;
    const int maxNameLength = 18;
    std::string str;

    sf::Text label;
    label.setPosition({x : 300, y : 200});
    label.setFont(getFont());
    label.setFillColor(sf::Color::White);
    label.setString("Enter your name");

    sf::Text playerName;
    playerName.setPosition({x : 265, y : 315});
    playerName.setFont(getFont());
    playerName.setFillColor(sf::Color::Yellow);
    playerName.setString("");

    sf::RectangleShape inputFieldBorder;
    inputFieldBorder.setSize(sf::Vector2f(330, 70));
    inputFieldBorder.setPosition(250, 300);
    inputFieldBorder.setFillColor(sf::Color::Red);

    sf::RectangleShape inputField;
    inputField.setSize(sf::Vector2f(324, 64));
    inputField.setPosition(253, 303);
    inputField.setFillColor(sf::Color::Black);

    sf::RectangleShape playButtonBorder;
    playButtonBorder.setSize(sf::Vector2f(106, 56));
    playButtonBorder.setPosition(347, 447);
    playButtonBorder.setFillColor(sf::Color::Red);

    sf::RectangleShape playButton;
    playButton.setSize(sf::Vector2f(100, 50));
    playButton.setPosition(350, 450);
    playButton.setFillColor(sf::Color::Yellow);

    sf::Text playText;
    playText.setPosition({x : 355, y : 455});
    playText.setFont(getFont());
    playText.setFillColor(sf::Color::Red);
    playText.setString("PLAY!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::TextEntered:
                if (event.key.code == 8) // backspace
                {
                    if (str.size() > 0)
                    {
                        str.resize(str.size() - 1);
                    }
                    playerName.setString(str);
                }
                else if (event.key.code == 13) //enter
                {
                    gameState = start_game;
                    plName = playerName.getString();
                    return;
                }
                else if (event.key.code < 128) //any other symbol
                {
                    if (str.size() < maxNameLength)
                    {
                        str += static_cast<char>(event.text.unicode);
                    }
                    playerName.setString(str);
                }
                break;
            case sf::Event::Closed:
                gameState = quit;
                plName = playerName.getString();
                return;
            }
            (sf::IntRect(350, 450, 100, 50).contains(sf::Mouse::getPosition(window))) ? hover = true : hover = false;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && hover)
            {
                gameState = start_game;
                plName = playerName.getString();
                return;
            }
        }
        window.clear();
        window.draw(label);
        window.draw(inputFieldBorder);
        window.draw(inputField);
        window.draw(playerName);
        if (hover)
        {
            window.draw(playButtonBorder);
        }
        window.draw(playButton);
        window.draw(playText);
        window.display();
    }
}