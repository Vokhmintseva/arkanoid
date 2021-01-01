sf::Texture platformTexture;
sf::Texture brickTexture;
sf::Texture ballTexture;
sf::Texture playerTexture;
sf::Texture scoresTexture;
sf::Texture bestScoresTexture;
sf::Texture backgroundTexture;
sf::Font font;

void loadAssets()
{
    platformTexture.loadFromFile("00/images/platform.png");
    ballTexture.loadFromFile("00/images/ball.png");
    brickTexture.loadFromFile("00/images/brick.png");
    playerTexture.loadFromFile("00/images/player.png");
    scoresTexture.loadFromFile("00/images/score.png");
    bestScoresTexture.loadFromFile("00/images/best-score.png");
    backgroundTexture.loadFromFile("00/images/background.png");
    font.loadFromFile("00/perfectDOSVGA437.ttf");
}

sf::Font &getFont()
{
    return font;
}

sf::Texture &getPlatformTexture()
{
    return platformTexture;
}

sf::Texture &getBrickTexture()
{
    return brickTexture;
}

sf::Texture &getBallTexture()
{
    return ballTexture;
}

sf::Texture &getPlayerTexture()
{
    return playerTexture;
}

sf::Texture &getScoresTexture()
{
    return scoresTexture;
}

sf::Texture &getBestScoresTexture()
{
    return bestScoresTexture;
}

sf::Texture &getBackgroundTexture()
{
    return backgroundTexture;
}