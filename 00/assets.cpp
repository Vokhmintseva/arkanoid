sf::Texture platformTexture;
sf::Texture brickTexture;
sf::Texture ballTexture;
sf::Texture playerTexture;
sf::Texture scoresTexture;
sf::Texture highScoresTexture;
sf::Texture highScoresLegendTexture;
sf::Texture trophyTexture;
sf::Texture backgroundTexture;
sf::Texture backgroundMenuTexture;
sf::Font font;

void loadAssets()
{
    platformTexture.loadFromFile("00/images/platform.png");
    ballTexture.loadFromFile("00/images/ball.png");
    brickTexture.loadFromFile("00/images/brick.png");
    playerTexture.loadFromFile("00/images/player.png");
    scoresTexture.loadFromFile("00/images/score.png");
    highScoresTexture.loadFromFile("00/images/high-score.png");
    highScoresLegendTexture.loadFromFile("00/images/high-score-legend.png");
    trophyTexture.loadFromFile("00/images/trophy.png");
    backgroundTexture.loadFromFile("00/images/background.png");
    backgroundMenuTexture.loadFromFile("00/images/backgrMenu.png");
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

sf::Texture &getHighScoresTexture()
{
    return highScoresTexture;
}

sf::Texture &getHighScoresLegendTexture()
{
    return highScoresLegendTexture;
}

sf::Texture &getTrophyTexture()
{
    return trophyTexture;
}

sf::Texture &getBackgroundTexture()
{
    return backgroundTexture;
}

sf::Texture &getBackgroundMenuTexture()
{
    return backgroundMenuTexture;
}