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
sf::Texture sapphireTexture;
sf::Texture rubyTexture;
sf::Texture stoneCrackedTexture;
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
    sapphireTexture.loadFromFile("00/images/sapphire.png");
    stoneCrackedTexture.loadFromFile("00/images/emerald_cracked.png");
    rubyTexture.loadFromFile("00/images/ruby.png");
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

sf::Texture &getSapphireTexture()
{
    return sapphireTexture;
}

sf::Texture &getRubyTexture()
{
    return rubyTexture;
}

sf::Texture &getStoneCrackedTexture()
{
    return stoneCrackedTexture;
}

sf::Font &getFont()
{
    return font;
}

sf::Texture &getPrizeSpriteTexture(PrizeType prizeType)
{
    switch (prizeType)
    {
    case expand_platform:
        return expandPlatformTexture;
    case two_balls:
        return twoBallsTexture;
    case slow_ball_down:
        return slowBallDownTexture;
    case accelerate_ball:
        return accelerateBallTexture;
    case extra_life:
        return extraLifeTexture;
    case portal_door:
        return portalDoorTexture;
    case sticky_ball:
        return stickyBallTexture;
    }
}

PrizeType getPrizeType(const sf::Texture *texture)
{
    if (texture == &expandPlatformTexture)
        return expand_platform;
    if (texture == &twoBallsTexture)
        return two_balls;
    if (texture == &slowBallDownTexture)
        return slow_ball_down;
    if (texture == &accelerateBallTexture)
        return accelerate_ball;
    if (texture == &extraLifeTexture)
        return extra_life;
    if (texture == &portalDoorTexture)
        return portal_door;
    if (texture == &stickyBallTexture)
        return sticky_ball;
}