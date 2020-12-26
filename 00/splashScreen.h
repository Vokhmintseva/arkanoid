void splashSreen(sf::RenderWindow &window)
{
    sf::Clock clock;

    const float windowWidth = window.getSize().x;
    const float windowHeight = window.getSize().y;
    sf::Texture logoTexture;
    sf::Sprite logoSprite;
    logoTexture.loadFromFile("00/images/arkanoid-logo.png");
    logoSprite.setTexture(logoTexture);
    logoSprite.setPosition({x : 0, y : 50});
    while (window.isOpen())
    {
        const float time = clock.getElapsedTime().asSeconds();
        if (time > 1)
        {
            return;
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        window.clear();
        window.draw(logoSprite);
        window.display();
    }
}