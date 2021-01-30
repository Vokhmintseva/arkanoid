#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include "calculations.cpp"
#include "scores.cpp"
#include "levels.cpp"

using namespace std;
void setPlatformSize();

const sf::Vector2f gameFieldPosition = sf::Vector2f(50, 50);
const float GAME_FIELD_WIDTH = 300;
const float GAME_FIELD_HEIGHT = 300;
const float LEFT_EDGE = gameFieldPosition.x;
const float RIGHT_EDGE = LEFT_EDGE + GAME_FIELD_WIDTH;
const float TOP = gameFieldPosition.y;
const float BOTTOM = TOP + GAME_FIELD_HEIGHT;
const float PLATFORM_WIDTH = 50;
const float PLATFORM_HEIGHT = 13;
const float INITIAL_PLATFORM_X = LEFT_EDGE + (GAME_FIELD_WIDTH - PLATFORM_WIDTH) / 2;
const float INITIAL_PLATFORM_Y = 325;
const float BALL_SIZE = 10;
const float INITIAL_BALL_X = LEFT_EDGE + (GAME_FIELD_WIDTH - BALL_SIZE) / 2; //195
const float INITIAL_BALL_Y = INITIAL_PLATFORM_Y - BALL_SIZE;                 //315
const float gameFieldOutlineThickness = 5;
float koeffOfPlatformExpansion = 1.0f;
float koeffOfBallSpeed = 1.0f;
const float regularBallSpeed = 100.0f;
float prizeStartTime;
const sf::Vector2f DOOR_POSITION = {RIGHT_EDGE, TOP + GAME_FIELD_HEIGHT / 4};
const float DOOR_WIDTH = 5;
const float DOOR_HEIGHT = GAME_FIELD_HEIGHT / 2;

sf::Sprite livesDesignation;
sf::Sprite ball;
sf::Sprite highScoresSprite;
sf::Sprite background;
sf::Sprite scoresSprite;
int ballXdir;
int ballYdir;
float ballSpeed;
bool isPaused;

sf::Text levelLostMsgTextModal;
sf::Text okTextModal;
sf::Text cancelTextModal;
sf::Text playerText;
sf::Text scoresText;
sf::Text highScoresText;
sf::RectangleShape levelLostModal;
int selectedModalItem = 1;

//std::vector<sf::Sprite *> activePrizes;
//std::vector<PrizeEffect> prizeEffects;

void resetPlatform()
{
    platform.setPosition(INITIAL_PLATFORM_X, INITIAL_PLATFORM_Y);
}

void resetBall()
{
    ball.setPosition(INITIAL_BALL_X, INITIAL_BALL_Y);
}

void handlePrize(sf::FloatRect brickBounds, PrizeType prizeType)
{
    sf::Sprite *spritePtr = new sf::Sprite();
    spritePtr->setTexture(getPrizeSpriteTexture(prizeType));
    spritePtr->setPosition(brickBounds.left, brickBounds.top);
    activePrizes.push_back(spritePtr);
}

void handleBallCollisionWithBrick(sf::FloatRect brickBounds, sf::FloatRect ballBounds, Brick brick)
{
    scores = scores + 50;
    scoresText.setString(std::to_string(scores));
    const float ballCenter = ballBounds.left + BALL_SIZE / 2;
    if (
        (brickBounds.left <= ballCenter) &&
        (ballCenter <= (brickBounds.left + brickBounds.width)))
    {
        ballYdir = -1 * ballYdir;
    }
    else if (
        (brickBounds.left > ballCenter) ||
        (ballCenter > (brickBounds.left + brickBounds.width)))
    {
        ballXdir = -1 * ballXdir;
    }

    if (brick.prize.prizeType != none && !brick.isBrokenWithDoubleHit)
    {
        handlePrize(brickBounds, brick.prize.prizeType);
    }
}

bool isStickyBallActive()
{
    for (int i = 0; i < prizeEffects.size(); i++)
    {
        if (prizeEffects[i].prizeEffectType == sticky_ball)
        {
            return true;
        }
    }
}

void handleBallCollisionWithPlatform(sf::FloatRect platformBounds, sf::FloatRect ballBounds, float &ballAngle, bool &isBallStuck)
{
    if ((ballBounds.top + ballBounds.height) >= (platformBounds.top + 3))
    {
        return;
    }
    if (isStickyBallActive())
    {
        isBallStuck = true;
    }
    const float ballCenter = ballBounds.left + BALL_SIZE / 2;
    const float point1 = platformBounds.left + PLATFORM_WIDTH / 4;
    const float point2 = platformBounds.left + PLATFORM_WIDTH / 2;
    const float point3 = platformBounds.left + PLATFORM_WIDTH / 4 * 3;
    if (ballCenter <= point1)
    {
        ballAngle = 45;
        ballYdir = -1 * abs(ballYdir);
        ballXdir = -1 * abs(ballXdir);
        return;
    }

    if (ballCenter <= point2 && ballCenter > point1)
    {
        ballAngle = 20;
        ballYdir = -1 * abs(ballYdir);
        ballXdir = -1 * abs(ballXdir);
        return;
    }

    if (ballCenter > point2 && ballCenter <= point3)
    {
        ballAngle = 20;
        ballYdir = -1 * abs(ballYdir);
        ballXdir = 1 * abs(ballXdir);
        return;
    }
    if (ballCenter > point3)
    {
        ballAngle = 45;
        ballYdir = -1 * abs(ballYdir);
        ballXdir = 1 * abs(ballXdir);
        return;
    }
}

void handleBallMiss(int &lives)
{
    decreaseScores();
    prizeEffects.clear();
    activePrizes.clear();
    lives--;
    if (lives < 1)
    {
        gameState = level_lost_modal;
        handleScores();
    }
    else
    {
        resetPlatform();
        setPlatformSize();
        resetBall();
        isPaused = true;
        ballXdir = 1;
        ballYdir = -1;
        ballSpeed = 100;
    }
}

void checkBallCollisionWithBrick(const sf::FloatRect &ballBounds, std::vector<Brick> &bricks, std::vector<float> *timeFromCollisionWithCrackedBrick,
                                 float &timeToShowLevelPassedMsg)
{
    for (int i = 0; i < bricks.size(); i++)
    {
        sf::Sprite brick = bricks[i].brickSprite;
        const sf::FloatRect brickBounds = brick.getGlobalBounds();
        if (ballBounds.intersects(brickBounds))
        {
            if (bricks[i].isBrokenWithDoubleHit)
            {
                bricks[i].brickSprite.setTexture(getStoneCrackedTexture());
            }
            if ((*timeFromCollisionWithCrackedBrick)[i] <= 0.0f)
            {
                handleBallCollisionWithBrick(brickBounds, ballBounds, bricks[i]);
                if (!bricks[i].isBrokenWithDoubleHit)
                    bricks.erase(bricks.begin() + i);
            }
            if (bricks[i].isBrokenWithDoubleHit)
            {
                bricks[i].isBrokenWithDoubleHit = false;
                timeFromCollisionWithCrackedBrick->insert(timeFromCollisionWithCrackedBrick->begin() + i, 0.15f);
            }
            if (bricks.empty())
            {
                level += 1;
                timeToShowLevelPassedMsg = 3.0f;
                gameState = level_passed;
            }
        }
    }
}

void checkBallCollisionWithPlatform(const sf::FloatRect &ballBounds, float &ballAngle, bool &isBallStuck)
{
    const sf::FloatRect platformBounds = platform.getGlobalBounds();
    if (ballBounds.intersects(platformBounds))
    {
        handleBallCollisionWithPlatform(platformBounds, ballBounds, ballAngle, isBallStuck);
    }
}

bool isPortalDoorActive()
{
    for (int i = 0; i < prizeEffects.size(); i++)
    {
        if (prizeEffects[i].prizeEffectType == portal_door)
        {
            return true;
        }
    }
}

void checkBallCollisionWithEdges(const sf::FloatRect &ballBounds, sf::Vector2f &newGlobalCoords, int &lives, float &timeToShowLevelPassedMsg)
{
    if (newGlobalCoords.x >= RIGHT_EDGE - BALL_SIZE)
    {
        if (isPortalDoorActive() && ballBounds.top >= DOOR_POSITION.y && (ballBounds.top + ballBounds.height <= DOOR_POSITION.y + DOOR_HEIGHT))
        {
            level++;
            gameState = level_passed;
            timeToShowLevelPassedMsg = 3.0f;
            return;
        }
        else
        {
            ballXdir = -1 * abs(ballXdir);
        }
    }
    if (newGlobalCoords.x <= LEFT_EDGE)
    {
        ballXdir = 1 * abs(ballXdir);
    }
    if (newGlobalCoords.y <= TOP)
    {
        ballYdir = 1 * abs(ballYdir);
    }
}

void checkBallMiss(sf::Vector2f &newGlobalCoords, int &lives)
{
    if (newGlobalCoords.y >= BOTTOM)
    {
        handleBallMiss(lives);
    }
}

void updateBall(float &ballSpeed, float &dt, std::vector<Brick> &bricks, std::vector<float> *timeFromCollisionWithCrackedBrick, int &lives,
                float &timeToShowLevelPassedMsg, float &ballAngle, bool &isBallStuck)
{
    if (isPaused || gameState != playing)
        return;
    const sf::FloatRect ballBounds = ball.getGlobalBounds();
    const sf::FloatRect platformBounds = platform.getGlobalBounds();
    checkBallCollisionWithPlatform(ballBounds, ballAngle, isBallStuck);
    sf::Vector2f newGlobalCoords;
    if (isBallStuck)
    {
        const float ballCenter = ballBounds.left + BALL_SIZE / 2;
        const float platformCenter = platformBounds.left + PLATFORM_WIDTH / 2;
        newGlobalCoords = {platformCenter, platformBounds.top - BALL_SIZE};
    }
    else
    {
        checkBallCollisionWithBrick(ballBounds, bricks, timeFromCollisionWithCrackedBrick, timeToShowLevelPassedMsg);
        const sf::Vector2f currentBallPos = ball.getPosition();
        const float S = ballSpeed * dt;
        sf::Vector2f newLocalCoords = toEuclidean(S, ballAngle);
        newLocalCoords.x = ballXdir * newLocalCoords.x;
        newLocalCoords.y = ballYdir * newLocalCoords.y;
        newGlobalCoords = currentBallPos + newLocalCoords;
        checkBallCollisionWithEdges(ballBounds, newGlobalCoords, lives, timeToShowLevelPassedMsg);
    }
    ball.setPosition(newGlobalCoords);
    checkBallMiss(newGlobalCoords, lives);
}

void updatePlatform(sf::Keyboard::Key &key, float &dt)
{
    const float speed = 1500;
    sf::Vector2f platformPosition = platform.getPosition();
    if (key == sf::Keyboard::Left)
    {
        if (platformPosition.x > gameFieldPosition.x)
            platform.move(-1 * speed * dt, 0);
    }
    if (key == sf::Keyboard::Right)
    {
        if (platformPosition.x < gameFieldPosition.x + GAME_FIELD_WIDTH - PLATFORM_WIDTH)
            platform.move(speed * dt, 0);
    }
}

void handleLeftKeyPressed(sf::Keyboard::Key &key, float &dt)
{
    if (gameState == playing && !isPaused)
    {
        updatePlatform(key, dt);
    }
    else
    {
        if (gameState == level_lost_modal)
        {
            selectedModalItem = 1;
            okTextModal.setStyle(sf::Text::Bold | sf::Text::Underlined);
            cancelTextModal.setStyle(sf::Text::Regular);
        }
    }
}

void handleRightKeyPressed(sf::Keyboard::Key &key, float &dt)
{
    if (gameState == playing && !isPaused)
    {
        updatePlatform(key, dt);
    }
    else
    {
        if (gameState == level_lost_modal)
        {
            selectedModalItem = 2;
            okTextModal.setStyle(sf::Text::Regular);
            cancelTextModal.setStyle(sf::Text::Bold | sf::Text::Underlined);
        }
    }
}

void handleReturnKeyReleased()
{
    scores = 0;
    if (gameState == level_lost_modal)
    {
        switch (selectedModalItem)
        {
        case 1:
            gameState = start_game;
            break;
        case 2:
            gameState = menu_screen;
            break;
        }
    }
}

void onKeyReleased(sf::Event::KeyEvent &key, float &dt, bool &isBallStuck)
{
    switch (key.code)
    {
    case sf::Keyboard::Space:
        isBallStuck ? isBallStuck = false : isPaused = !isPaused;
        break;
    case sf::Keyboard::Return:
        handleReturnKeyReleased();
        return;
    default:
        break;
    }
}

void onKeyPressed(sf::Event::KeyEvent &key, float &dt)
{
    switch (key.code)
    {
    case sf::Keyboard::Left:
        handleLeftKeyPressed(key.code, dt);
        break;
    case sf::Keyboard::Right:
        handleRightKeyPressed(key.code, dt);
        break;
    default:
        break;
    }
}

//опрашивает и обрабатывает доступные события в цикле
void pollEvents(sf::RenderWindow &window, float &dt, bool &isBallStuck)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            handleScores();
            gameState = quit;
            return;
        case sf::Event::KeyReleased:
            onKeyReleased(event.key, dt, isBallStuck);
            break;
        case sf::Event::KeyPressed:
            onKeyPressed(event.key, dt);
            break;
        default:
            break;
        }
    }
}

void drawBricks(sf::RenderWindow &window, std::vector<Brick> &bricks)
{
    for (std::vector<int>::size_type i = 0; i != bricks.size(); i++)
    {
        window.draw(bricks[i].brickSprite);
    }
}

void drawLevelLostModal(sf::RenderWindow &window)
{
    window.draw(levelLostModal);
    window.draw(levelLostMsgTextModal);
    window.draw(okTextModal);
    window.draw(cancelTextModal);
}

bool isExtraLifePrizeActive()
{
    for (int i = 0; i < prizeEffects.size(); i++)
    {
        if (prizeEffects[i].prizeEffectType == extra_life)
        {
            return true;
        }
    }
}

void drawSidebar(sf::RenderWindow &window, int &lives, sf::Sprite playerSprite, sf::RectangleShape livesDesignationBackgr)
{
    window.draw(playerText);
    window.draw(playerSprite);
    window.draw(scoresText);
    window.draw(scoresSprite);
    sf::Vector2f initPos = {x : 370, y : 310};
    livesDesignationBackgr.setPosition(362, 310 + 22 * (lives - 1));
    if (isExtraLifePrizeActive())
    {
        window.draw(livesDesignationBackgr);
    }
    for (int i = 0; i < lives; i++)
    {
        livesDesignation.setPosition(initPos);
        window.draw(livesDesignation);
        initPos.y = initPos.y + 22;
    }
    window.draw(highScoresSprite);
    window.draw(highScoresText);
}

void drawPrizes(sf::RenderWindow &window)
{
    for (sf::Sprite *prizeSprite : activePrizes)
    {
        window.draw(*prizeSprite);
    }
}

void drawLevelPassedModal(sf::RenderWindow &window, sf::Text *levelPassedText)
{
    window.draw(levelLostModal);
    window.draw(*levelPassedText);
}

void redrawFrame(sf::RenderWindow &window, std::vector<Brick> bricks, sf::RectangleShape field, sf::RectangleShape door, int &lives,
                 sf::Sprite playerSprite, sf::RectangleShape livesDesignationBackgr, sf::Text *levelPassedText)
{
    window.clear(sf::Color::White);
    window.draw(background);
    window.draw(field);
    drawBricks(window, bricks);
    drawSidebar(window, lives, playerSprite, livesDesignationBackgr);
    window.draw(platform);
    window.draw(ball);
    drawPrizes(window);
    if (gameState == level_lost_modal)
    {
        drawLevelLostModal(window);
    }
    if (isPortalDoorActive())
    {
        window.draw(door);
    }
    if (gameState == level_passed)
    {
        drawLevelPassedModal(window, levelPassedText);
    }
    window.display();
}

sf::RectangleShape createGameField()
{
    sf::RectangleShape gameField;
    gameField.setSize({x : GAME_FIELD_WIDTH, y : GAME_FIELD_HEIGHT});
    gameField.setOutlineColor(sf::Color(76, 5, 73));
    gameField.setOutlineThickness(gameFieldOutlineThickness);
    gameField.setPosition(gameFieldPosition);
    gameField.setFillColor(sf::Color(50, 25, 100, 180));
    return gameField;
}

sf::RectangleShape createDoor()
{
    sf::RectangleShape door;
    door.setSize({x : DOOR_WIDTH, y : DOOR_HEIGHT});
    door.setOutlineColor(sf::Color(197, 21, 21));
    door.setPosition(DOOR_POSITION);
    door.setFillColor(sf::Color(197, 21, 21));
    return door;
}

void recalcTimeToShowLevelPassedMsg(float &dt, float &timeToShowLevelPassedMsg, sf::Text *textPtr)
{
    if (gameState == level_passed)
    {
        timeToShowLevelPassedMsg -= dt;
        if (timeToShowLevelPassedMsg <= 0)
        {
            gameState = start_game;
            delete textPtr;
        }
    }
}

void setPlatformSize()
{
    sf::Vector2f platformSize(PLATFORM_WIDTH, PLATFORM_HEIGHT);
    platform.setScale(
        platformSize.x / platform.getLocalBounds().width,
        platformSize.y / platform.getLocalBounds().height);
}

void adjustPlatform()
{
    platform.setTexture(getPlatformTexture());
    setPlatformSize();
    resetPlatform();
}

void adjustBall()
{
    ball.setTexture(getBallTexture());
    ball.setScale(
        BALL_SIZE / ball.getLocalBounds().width,
        BALL_SIZE / ball.getLocalBounds().height);
    ballXdir = 1;
    ballYdir = -1;
    ballSpeed = 100.0f;
    resetBall();
}

sf::RectangleShape setLivesDesignationBackgr()
{
    sf::RectangleShape livesDesignationBackgr;
    livesDesignationBackgr.setSize({x : 56, y : 20});
    livesDesignationBackgr.setOutlineColor(sf::Color(197, 21, 21));
    livesDesignationBackgr.setFillColor(sf::Color(197, 21, 21));
    livesDesignationBackgr.setRotation(-20);
    return livesDesignationBackgr;
}

sf::Sprite adjustPlayer()
{
    sf::Sprite playerSprite;
    playerSprite.setTexture(getPlayerTexture());
    playerSprite.setPosition(370, 50);
    return playerSprite;
}

void adjustSidebar()
{
    playerText = sf::Text();
    playerText.setFillColor(sf::Color(232, 6, 107));
    playerText.setFont(getFont());
    playerText.setCharacterSize(30);
    playerText.setString(playerName);
    playerText.setPosition({x : 440, y : 80});

    scoresSprite.setTexture(getScoresTexture());
    scoresSprite.setPosition(370, 190);

    scoresText = sf::Text();
    scoresText.setFillColor(sf::Color::Red);
    scoresText.setFont(getFont());
    scoresText.setCharacterSize(50);
    scoresText.setString(std::to_string(scores));
    scoresText.setPosition({x : 450, y : 192});

    livesDesignation.setTexture(getPlatformTexture());
    sf::Vector2f livesDesignationSize(40, 15);
    livesDesignation.setScale(
        livesDesignationSize.x / livesDesignation.getLocalBounds().width,
        livesDesignationSize.y / livesDesignation.getLocalBounds().height);
    livesDesignation.setRotation(-20);

    highScoresSprite.setTexture(getHighScoresTexture());
    highScoresSprite.setPosition(370, 430);

    highScoresText = sf::Text();
    highScoresText.setFillColor(sf::Color::Yellow);
    highScoresText.setFont(getFont());
    highScoresText.setCharacterSize(50);
    highScoresText.setString(highScoresStr);
    highScoresText.setPosition({x : 450, y : 440});
}

void adjustGameLostModal()
{
    levelLostModal.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    levelLostModal.setPosition(0, 0);
    levelLostModal.setFillColor(sf::Color(0, 0, 0, 200));

    levelLostMsgTextModal = sf::Text();
    levelLostMsgTextModal.setFillColor(sf::Color::Red);
    levelLostMsgTextModal.setStyle(sf::Text::Bold);
    levelLostMsgTextModal.setCharacterSize(25);
    levelLostMsgTextModal.setString("YOU HAVE LOST THE LEVEL.\nWOULD YOU LIKE TO REPEAT ONE?");
    levelLostMsgTextModal.setPosition({x : WINDOW_WIDTH / 4, y : WINDOW_HEIGHT / 3});

    okTextModal = sf::Text();
    okTextModal.setFillColor(sf::Color::Red);
    okTextModal.setStyle(sf::Text::Bold | sf::Text::Underlined);
    okTextModal.setCharacterSize(25);
    okTextModal.setString("OK");
    okTextModal.setPosition({x : WINDOW_WIDTH * 1 / 3, y : WINDOW_HEIGHT / 2});

    cancelTextModal = sf::Text();
    cancelTextModal.setFillColor(sf::Color::Red);
    cancelTextModal.setStyle(sf::Text::Regular);
    cancelTextModal.setCharacterSize(25);
    cancelTextModal.setString("CANCEL");
    cancelTextModal.setPosition({x : WINDOW_WIDTH * 2 / 3, y : WINDOW_HEIGHT / 2});

    levelLostMsgTextModal.setFont(getFont());
    okTextModal.setFont(getFont());
    cancelTextModal.setFont(getFont());
}

sf::Text *getLevelPassedText()
{
    sf::Text *textPtr = new sf::Text();
    textPtr->setFillColor(sf::Color::Red);
    textPtr->setStyle(sf::Text::Bold);
    textPtr->setCharacterSize(35);
    textPtr->setString("YOU HAVE PASSED THE LEVEL!");
    textPtr->setPosition({x : WINDOW_WIDTH / 8, y : WINDOW_HEIGHT / 2});
    textPtr->setFont(getFont());
    return textPtr;
}

void resetGlobalVars()
{
    isPaused = true;
    int selectedModalItem = 1;
    gameState = playing;
    highScoresStr = "";
    prizeEffects.clear();
    activePrizes.clear();
}

void loadPrizesTextures()
{
    expandPlatformSprite.setTexture(getExpandPlatformTexture());
    twoBallsSprite.setTexture(getTwoBallsTexture());
    slowBallDownSprite.setTexture(getSlowBallDownTexture());
    accelerateBallSprite.setTexture(getAccelerateBallTexture());
    extraLifeSprite.setTexture(getExtraLifeTexture());
    portalDoorSprite.setTexture(getPortalDoorTexture());
    stickyBallSprite.setTexture(getStickyBallTexture());
}

void changePlatformSize(float koeff)
{
    sf::Vector2f platformSize(PLATFORM_WIDTH * koeff, PLATFORM_HEIGHT);
    platform.setScale(
        platformSize.x / platform.getLocalBounds().width,
        platformSize.y / platform.getLocalBounds().height);
}

void changeBallSpeed(float koeff)
{
    ballSpeed = regularBallSpeed * koeff;
}

void twoBalls()
{
}

void extraLife()
{
}

void portalDoor()
{
}

void stickyBall()
{
}

void applyPrizeEffect(PrizeType prizeType, int &lives)
{
    switch (prizeType)
    {
    case expand_platform:
        koeffOfPlatformExpansion += 0.35;
        if (koeffOfPlatformExpansion < 2)
            changePlatformSize(koeffOfPlatformExpansion);
        break;
    case accelerate_ball:
        koeffOfBallSpeed += 0.35;
        if (koeffOfBallSpeed < 2)
            changeBallSpeed(koeffOfBallSpeed);
        break;
    case slow_ball_down:
        koeffOfBallSpeed -= 0.35;
        if (koeffOfBallSpeed > 0)
            changeBallSpeed(koeffOfBallSpeed);
        break;
    case two_balls:
        twoBalls();
        break;
    case extra_life:
        lives++;
        break;
    case portal_door:
        portalDoor();
        break;
    case sticky_ball:
        stickyBall();
        break;
    }
}

PrizeType getPrizeType(sf::Sprite *prizeSprite)
{
    const sf::Texture *prizeTexture = prizeSprite->getTexture();
    return getPrizeType(prizeTexture);
}

void deletePtr(sf::Sprite *prizeSprite)
{
    prizeSprite = NULL;
    delete prizeSprite;
}

void deletePrizeFromActivePrizes(sf::Sprite *prizeSprite)
{
    activePrizes.erase(std::remove(activePrizes.begin(), activePrizes.end(), prizeSprite), activePrizes.end());
    deletePtr(prizeSprite);
}

void handlePrizeCollisionWithPlatform(sf::Sprite *prizeSprite, float &dt, int &lives)
{
    prizeStartTime = dt;
    PrizeType prizeType = getPrizeType(prizeSprite);
    applyPrizeEffect(prizeType, lives);
    deletePrizeFromActivePrizes(prizeSprite);
    PrizeEffect effect;
    effect.prizeEffectType = getPrizeType(prizeSprite);
    effect.timeOfEffectApplying = 0;
    prizeEffects.push_back(effect);
}

void checkPrizeCollisionWithPlatform(float &dt, int &lives)
{
    const sf::FloatRect platformBounds = platform.getGlobalBounds();
    for (sf::Sprite *prizeSprite : activePrizes)
    {
        const sf::FloatRect prizeBounds = prizeSprite->getGlobalBounds();
        if (prizeBounds.intersects(platformBounds))
        {
            handlePrizeCollisionWithPlatform(prizeSprite, dt, lives);
        }
    }
}

void checkPrizeMiss()
{
    for (sf::Sprite *prizeSprite : activePrizes)
    {
        const sf::FloatRect prizeBounds = prizeSprite->getGlobalBounds();
        const sf::Vector2f currPrizeSpritePos = prizeSprite->getPosition();
        if (currPrizeSpritePos.y + prizeBounds.height > BOTTOM)
            deletePrizeFromActivePrizes(prizeSprite);
    }
}

void updateFallingPrizes(float &dt, int &lives)
{
    if (isPaused || gameState != playing)
        return;
    const float prizeSpeed = 40;
    for (sf::Sprite *prizeSprite : activePrizes)
    {
        sf::Vector2f currPos = prizeSprite->getPosition();
        const float newYpos = currPos.y + dt * prizeSpeed;
        prizeSprite->setPosition(currPos.x, newYpos);
    }
    checkPrizeCollisionWithPlatform(dt, lives);
    checkPrizeMiss();
}

void undoEffect(PrizeType prizeType)
{
    switch (prizeType)
    {
    case expand_platform:
        koeffOfPlatformExpansion -= 0.35;
        if (koeffOfPlatformExpansion > 0)
            changePlatformSize(koeffOfPlatformExpansion);
        break;
    case accelerate_ball:
        koeffOfBallSpeed -= 0.35;
        if (koeffOfBallSpeed > 0)
            changeBallSpeed(koeffOfBallSpeed);
        break;
    case slow_ball_down:
        koeffOfBallSpeed += 0.35;
        if (koeffOfBallSpeed < 2)
            changeBallSpeed(koeffOfBallSpeed);
        break;
    }
}

void updatePrizeEffects(float &dt)
{
    if (isPaused || gameState != playing)
        return;
    for (int i = 0; i < prizeEffects.size(); i++)
    {
        const float effectTime = prizeEffects[i].timeOfEffectApplying;
        const float newEffectTime = effectTime + dt;
        prizeEffects[i].timeOfEffectApplying = newEffectTime;

        if (prizeEffects[i].timeOfEffectApplying > regularTimeOfPrizeEffect)
        {
            undoEffect(prizeEffects[i].prizeEffectType);
            prizeEffects.erase(prizeEffects.begin() + i);
        }
    }
}

bool shouldLeaveGame()
{
    return (gameState == quit ||
            gameState == menu_screen ||
            gameState == start_game);
}

updateArrOfTimeFromCollision(std::vector<float> *timeFromCollisionWithCrackedBrick, float dt)
{
    for (auto it = timeFromCollisionWithCrackedBrick->begin(); it != timeFromCollisionWithCrackedBrick->end(); ++it)
    {
        if (*it == 0)
        {
            continue;
        }
        *it -= dt;
        if (*it <= 0)
        {
            (*it = 0);
        }
    }
}

void playGame(sf::RenderWindow &window)
{
    resetGlobalVars();
    sf::Sprite playerSprite = adjustPlayer();
    sf::Text *levelPassedText = getLevelPassedText();
    sf::RectangleShape livesDesignationBackgr = setLivesDesignationBackgr();
    getBestScores();
    background.setTexture(getBackgroundMainTexture());
    adjustPlatform();
    adjustBall();
    adjustGameLostModal();
    adjustSidebar();
    loadPrizesTextures();
    std::vector<Brick> bricks;
    float timeToShowLevelPassedMsg = 0;
    std::vector<float> timeFromCollisionWithCrackedBrick;
    sf::RectangleShape door = createDoor();
    bool isBallStuck = false;
    float ballAngle = 45;

    int lives = 10;
    switch (level)
    {
    case 1:
        bricks = createBricksVector_1level({x : 70, y : 60}, &timeFromCollisionWithCrackedBrick);
        break;
    case 2:
        bricks = createBricksVector_2level({x : 65, y : 60}, &timeFromCollisionWithCrackedBrick);
        break;
    case 3:
        bricks = createBricksVector_3level({x : 65, y : 60}, &timeFromCollisionWithCrackedBrick);
        break;
    default:
        gameState = menu_screen;
    }
    sf::RectangleShape gameField = createGameField();
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        pollEvents(window, dt, isBallStuck);
        updateBall(ballSpeed, dt, bricks, &timeFromCollisionWithCrackedBrick, lives, timeToShowLevelPassedMsg, ballAngle, isBallStuck);
        updateFallingPrizes(dt, lives);
        updateArrOfTimeFromCollision(&timeFromCollisionWithCrackedBrick, dt);
        updatePrizeEffects(dt); //обновить время действия эффектов от призов
        recalcTimeToShowLevelPassedMsg(dt, timeToShowLevelPassedMsg, levelPassedText);
        redrawFrame(window, bricks, gameField, door, lives, playerSprite, livesDesignationBackgr, levelPassedText);
        if (shouldLeaveGame())
        {
            cout << "return" << endl;
            cout << gameState << endl;
            return;
        }
    }
}

//sf::Keyboard::isKeyPressed(sf::Keyboard::Space) : isGameStarted = true;