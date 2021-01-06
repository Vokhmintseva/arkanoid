sf::Texture platformTexture;
sf::Texture brickTexture;
sf::Texture ballTexture;
sf::Texture playerTexture;
sf::Texture scoresTexture;
sf::Texture highScoresTexture;
sf::Texture highScoresLegendTexture;
sf::Texture trophyTexture;
sf::Texture backgroundMainTexture;
sf::Texture backgroundMenuTexture;
sf::Texture expandPlatformTexture;
sf::Texture twoBallsTexture;
sf::Texture slowBallDownTexture;
sf::Texture accelerateBallTexture;
sf::Texture extraLifeTexture;
sf::Texture portalDoorTexture;
sf::Texture stickyBallTexture;

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
    backgroundMainTexture.loadFromFile("00/images/backgroundMain.png");
    backgroundMenuTexture.loadFromFile("00/images/backgrMenu.png");
    expandPlatformTexture.loadFromFile("00/images/expand.png");
    twoBallsTexture.loadFromFile("00/images/ballX2.png");
    slowBallDownTexture.loadFromFile("00/images/snail.png");
    accelerateBallTexture.loadFromFile("00/images/acceleration.png");
    extraLifeTexture.loadFromFile("00/images/heart.png");
    portalDoorTexture.loadFromFile("00/images/door.png");
    stickyBallTexture.loadFromFile("00/images/duct-tape.png");

    font.loadFromFile("00/perfectDOSVGA437.ttf");
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

sf::Texture &getBackgroundMainTexture()
{
    return backgroundMainTexture;
}

sf::Texture &getBackgroundMenuTexture()
{
    return backgroundMenuTexture;
}

sf::Texture &getExpandPlatformTexture()
{
    return expandPlatformTexture;
}

sf::Texture &getTwoBallsTexture()
{
    return twoBallsTexture;
}

sf::Texture &getSlowBallDownTexture()
{
    return slowBallDownTexture;
}

sf::Texture &getAccelerateBallTexture()
{
    return accelerateBallTexture;
}

sf::Texture &getExtraLifeTexture()
{
    return extraLifeTexture;
}

sf::Texture &getPortalDoorTexture()
{
    return portalDoorTexture;
}

sf::Texture &getStickyBallTexture()
{
    return stickyBallTexture;
}

sf::Font &getFont()
{
    return font;
}