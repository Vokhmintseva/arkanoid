#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;
void setPlatformSize();

sf::Sprite expandPlatformSprite;
sf::Sprite twoBallsSprite;
sf::Sprite slowBallDownSprite;
sf::Sprite accelerateBallSprite;
sf::Sprite extraLifeSprite;
sf::Sprite portalDoorSprite;
sf::Sprite stickyBallSprite;

struct Prize
{
    PrizeType prizeType;
    sf::Sprite *prizeSprite;
};
struct Brick
{
    sf::Sprite brickSprite;
    Prize prize;
};

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
int scores;

sf::Sprite platform;
sf::Sprite livesDesignation;
sf::Sprite brick;
sf::Sprite ball;
sf::Sprite highScoresSprite;
sf::Sprite player;
sf::Sprite background;
sf::Sprite scoresSprite;

int ballXdir;
int ballYdir;
float ballSpeed;
bool isPaused;
int lives;
sf::Text levelLostMsgTextModal;
sf::Text okTextModal;
sf::Text cancelTextModal;
sf::Text playerText;
sf::Text scoresText;
sf::Text highScoresText;
sf::RectangleShape levelLostModal;
int selectedModalItem = 1;
std::string highScoresStr;
std::vector<sf::Sprite *> activePrizes;
std::vector<PrizeEffect> prizeEffects;

std::map<PrizeType, sf::Sprite *> prizesSprites = {
    {expand_platform, &expandPlatformSprite},
    {two_balls, &twoBallsSprite},
    {slow_ball_down, &slowBallDownSprite},
    {accelerate_ball, &accelerateBallSprite},
    {extra_life, &extraLifeSprite},
    {portal_door, &portalDoorSprite},
    {sticky_ball, &stickyBallSprite}};

// std::map<PrizeType, PrizeStates> prizeGameStates = {
//     {expand_platform, expand_platform_prize_state},
//     {two_balls, two_balls_prize_state},
//     {slow_ball_down, slow_ball_down_prize_state},
//     {accelerate_ball, accelerate_ball_prize_state},
//     {extra_life, extra_life_prize_state},
//     {portal_door, portal_door_prize_state},
//     {sticky_ball, sticky_ball_prize_state}};

//из полярных в евклидовы координаты
sf::Vector2f toEuclidean(float radius, float angle)
{
    return {
        radius * std::cos(angle),
        radius * std::sin(angle)};
}

//из радиан в градусы
float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void resetPlatform()
{
    platform.setPosition(INITIAL_PLATFORM_X, INITIAL_PLATFORM_Y);
}

void resetBall()
{
    ball.setPosition(INITIAL_BALL_X, INITIAL_BALL_Y);
}

Brick createBrick(sf::Color color, sf::Vector2f position, map<int, PrizeType> prizesAssignment, const int index)
{
    brick.setPosition(position);
    brick.setColor(color);
    Brick oneBrick;
    // oneBrick.sprite.setPosition(position);
    // oneBrick.sprite.setColor(color);
    oneBrick.brickSprite = brick;
    //oneBrick.position = position;
    if (prizesAssignment.count(index))
    {
        oneBrick.brickSprite.setColor(sf::Color(41, 255, 0));
        PrizeType prize = prizesAssignment[index];
        oneBrick.prize.prizeType = prize;
        oneBrick.prize.prizeSprite = prizesSprites[prize];
    }
    else
    {
        oneBrick.prize.prizeType = none;
    }
    return oneBrick;
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
    if (brick.prize.prizeType != none)
    {
        handlePrize(brickBounds, brick.prize.prizeType);
    }
}

void handleBallCollisionWithPlatform(sf::FloatRect platformBounds, sf::FloatRect ballBounds)
{
    const float ballCenter = ballBounds.left + BALL_SIZE / 2;
    const float platformCenter = platformBounds.left + PLATFORM_WIDTH / 2;
    if (ballCenter <= platformCenter)
    {
        ballYdir = -1 * abs(ballYdir);
        ballXdir = -1 * abs(ballXdir);
    }
    else
    {
        ballYdir = -1 * abs(ballYdir);
        ballXdir = 1 * abs(ballXdir);
    }
}

void decreaseScores()
{
    scores >= 50
        ? scores = scores - 50
        : scores = 0;
}

void handleBallMiss()
{
    decreaseScores();
    lives--;
    if (lives < 1)
    {
        gameState = level_lost_modal;
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

void checkBallColiisionWithBrick(const sf::FloatRect &ballBounds, std::vector<Brick> &bricks)
{
    for (int i = 0; i < bricks.size(); i++)
    {
        sf::Sprite brick = bricks[i].brickSprite;
        const sf::FloatRect brickBounds = brick.getGlobalBounds();
        if (ballBounds.intersects(brickBounds))
        {
            handleBallCollisionWithBrick(brickBounds, ballBounds, bricks[i]);
            bricks.erase(bricks.begin() + i);
        }
    }
}

void checkBallColiisionWithPlatform(const sf::FloatRect &ballBounds)
{
    const sf::FloatRect platformBounds = platform.getGlobalBounds();
    if (ballBounds.intersects(platformBounds))
    {
        handleBallCollisionWithPlatform(platformBounds, ballBounds);
    }
}

void checkBallColiisionWithEdges(const sf::FloatRect &ballBounds, sf::Vector2f &newGlobalCoords)
{
    if (newGlobalCoords.x >= RIGHT_EDGE - BALL_SIZE)
    {
        ballXdir = -1 * abs(ballXdir);
    }
    if (newGlobalCoords.x <= LEFT_EDGE)
    {
        ballXdir = 1 * abs(ballXdir);
    }
    if (newGlobalCoords.y <= TOP)
    {
        ballYdir = 1 * abs(ballYdir);
    }
    ball.setPosition(newGlobalCoords);
    if (newGlobalCoords.y >= BOTTOM)
    {
        handleBallMiss();
    }
}

void updateBall(float &ballSpeed, float &dt, std::vector<Brick> &bricks)
{
    if (isPaused || gameState != playing)
        return;
    const sf::FloatRect ballBounds = ball.getGlobalBounds();
    checkBallColiisionWithBrick(ballBounds, bricks);
    checkBallColiisionWithPlatform(ballBounds);
    const float angle = 45;
    const sf::Vector2f currentBallPos = ball.getPosition();
    const float S = ballSpeed * dt;
    sf::Vector2f newLocalCoords = toEuclidean(S, angle);
    newLocalCoords.x = ballXdir * newLocalCoords.x;
    newLocalCoords.y = ballYdir * newLocalCoords.y;
    sf::Vector2f newGlobalCoords = currentBallPos + newLocalCoords;
    checkBallColiisionWithEdges(ballBounds, newGlobalCoords);
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

void onKeyReleased(sf::Event::KeyEvent &key, float &dt)
{
    switch (key.code)
    {
    case sf::Keyboard::Space:
        isPaused = !isPaused;
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
void pollEvents(sf::RenderWindow &window, float &dt)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            gameState = quit;
            return;
        case sf::Event::KeyReleased:
            onKeyReleased(event.key, dt);
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

void drawSidebar(sf::RenderWindow &window)
{
    window.draw(playerText);
    window.draw(player);
    window.draw(scoresText);
    window.draw(scoresSprite);
    sf::Vector2f initPos = {x : 370, y : 310};
    //cout << lives << endl;
    for (int i = 0; i < lives; i++)
    {
        livesDesignation.setPosition(initPos);
        window.draw(livesDesignation);
        initPos.y = initPos.y + 30;
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

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, std::vector<Brick> bricks, sf::RectangleShape field)
{
    window.clear(sf::Color::White);
    window.draw(background);
    window.draw(field);
    drawBricks(window, bricks);
    drawSidebar(window);
    window.draw(platform);
    window.draw(ball);
    drawPrizes(window);
    if (gameState == level_lost_modal)
    {
        drawLevelLostModal(window);
    }
    window.display();
}

int getRandomNumber(int min, int max)
{
    // const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    // return static_cast<int>(rand() * fraction * (max - min + 1) + min);
    return rand() % (max + 1);
}

int changeBrickIndex(int index, const int bricksNumber)
{
    index = index + 1;
    if (index > bricksNumber - 1)
    {
        index = index - 2;
    }
    return index;
}

vector<int> definePrizesBricksIndexes(const int bricksNumber, const int numberOfRandoms)
{
    srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
    vector<int> prizeBricksIndexes;
    while (prizeBricksIndexes.size() < numberOfRandoms)
    {
        int index = getRandomNumber(0, bricksNumber - 1);
        while (std::find(prizeBricksIndexes.begin(), prizeBricksIndexes.end(), index) != prizeBricksIndexes.end())
            index = changeBrickIndex(index, bricksNumber);
        prizeBricksIndexes.push_back(index);
        //std::cout << index << "\t";
    }
    //std::cout << std::endl;
    prizeBricksIndexes = {24, 25, 26, 27, 28, 29};
    return prizeBricksIndexes;
}

int getBrickIndex(int row, int column, const int columnsTotal)
{
    return (row * columnsTotal + column);
}

std::vector<Brick> createBricksVector_1level(sf::Vector2f startPosition)
{
    const int bricksNumber = 30;
    const int bricksWithPrizesNum = 6;
    vector<int> prizeBricksIndexes = definePrizesBricksIndexes(bricksNumber, bricksWithPrizesNum);
    map<int, PrizeType> prizesAssignment = {
        {prizeBricksIndexes[0], slow_ball_down},
        {prizeBricksIndexes[1], slow_ball_down},
        {prizeBricksIndexes[2], slow_ball_down},
        {prizeBricksIndexes[3], slow_ball_down},
        {prizeBricksIndexes[4], slow_ball_down},
        {prizeBricksIndexes[5], slow_ball_down}};
    brick.setTexture(getBrickTexture());
    std::vector<Brick> bricks;
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    const int rowsTotal = 5;
    const int columnsTotal = 6;
    for (int i = 0; i < rowsTotal; i++)
    {
        for (int j = 0; j < columnsTotal; j++)
        {
            const int index = getBrickIndex(i, j, columnsTotal);
            bricks.push_back(createBrick(sf::Color(250, 107, 235), {x : xStart, y : yStart}, prizesAssignment, index));
            xStart = xStart + 45;
        }
        xStart = startPosition.x;
        yStart = yStart + 25;
    }
    // for (int i = 0; i < bricksNumber; i++)
    // {
    //     cout << bricks[i].prize << "\t";
    // }
    return bricks;
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

void setTimeToShowFailMsg(float &dt, float &timeToShowFailMsg)
{
    timeToShowFailMsg = timeToShowFailMsg + dt;
    if (timeToShowFailMsg > 3)
    {
        gameState = level_lost;
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

void adjustSidebar()
{
    player.setTexture(getPlayerTexture());
    player.setPosition(370, 50);

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

void resetGlobalVars()
{
    isPaused = true;
    lives = 2;
    int selectedModalItem = 1;
    gameState = playing;
    highScoresStr = "";
    prizeStartTime = 0;
    scores = 0;
}

void getBestScores()
{
    std::ifstream fileout("HighScores.txt"); // окрываем файл для чтения
    if (fileout.is_open())
    {
        std::string line;
        if (getline(fileout, line))
        {
            size_t i = line.find(' ');
            highScoresStr = line.substr(0, i);
        }
    }
    fileout.close();
}

bool comparePlayers(const Player &a, const Player &b)
{
    return a.scores >= b.scores;
}

void readHighScores(std::list<Player> &bestScores)
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

void writeHighScores(std::list<Player> &bestScores)
{
    int numberOfRecords = 5;
    std::ofstream filein;          // поток для записи
    filein.open("HighScores.txt"); // окрываем файл для записи
    bestScores.sort(comparePlayers);
    for (Player player : bestScores)
    {
        filein << player.scores << " " << player.name << endl;
        numberOfRecords--;
        if (numberOfRecords < 1)
            break;
    }
    filein.close();
}

void handleScores()
{
    std::list<Player> bestScores;
    if (scores != 0)
    {
        Player currPlayer = {scores, playerName};
        bestScores.push_front(currPlayer);
    }
    readHighScores(bestScores);
    writeHighScores(bestScores);
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

void applyPrizeEffect(PrizeType prizeType)
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
        {
            cout << "apply slow down ball speed calling" << endl;
            changeBallSpeed(koeffOfBallSpeed);
        }
        break;
    case two_balls:
        twoBalls();
        break;
    case extra_life:
        extraLife();
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

void handlePrizeCollisionWithPlatform(sf::Sprite *prizeSprite, float &dt)
{
    prizeStartTime = dt;
    PrizeType prizeType = getPrizeType(prizeSprite);
    applyPrizeEffect(prizeType);
    deletePrizeFromActivePrizes(prizeSprite);
    PrizeEffect effect;
    effect.prizeEffectType = getPrizeType(prizeSprite);
    effect.timeOfEffectApplying = 0;
    prizeEffects.push_back(effect);
}

void checkPrizeCollisionWithPlatform(float &dt)
{
    const sf::FloatRect platformBounds = platform.getGlobalBounds();
    for (sf::Sprite *prizeSprite : activePrizes)
    {
        const sf::FloatRect prizeBounds = prizeSprite->getGlobalBounds();
        if (prizeBounds.intersects(platformBounds))
        {
            handlePrizeCollisionWithPlatform(prizeSprite, dt);
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

void updatePrizes(float &dt)
{
    const float prizeSpeed = 10;
    for (sf::Sprite *prizeSprite : activePrizes)
    {
        sf::Vector2f currPos = prizeSprite->getPosition();
        const float newYpos = currPos.y + dt * prizeSpeed;
        prizeSprite->setPosition(currPos.x, newYpos);
    }
    checkPrizeCollisionWithPlatform(dt);
    checkPrizeMiss();
}

void closePortalDoor()
{
}

void nonStickyBall()
{
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
        {
            cout << "undo slow down ball speed calling" << endl;
            changeBallSpeed(koeffOfBallSpeed);
        }
        break;
    // case two_balls_prize_state:
    //     twoBalls();
    //     break;
    // case extra_life_prize_state:
    //     extraLife();
    //     break;
    case portal_door:
        closePortalDoor();
        break;
    case sticky_ball:
        nonStickyBall();
        break;
    }
}

void updatePrizeEffects(float &dt)
{
    //cout << prizeEffects.size() << endl;
    for (int i = 0; i < prizeEffects.size(); i++)
    {
        const float effectTime = prizeEffects[i].timeOfEffectApplying;
        const float newEffectTime = effectTime + dt;
        //cout << "reg time of prize" << newEffectTime << endl;
        prizeEffects[i].timeOfEffectApplying = newEffectTime;

        if (prizeEffects[i].timeOfEffectApplying > regularTimeOfPrizeEffect)
        {
            //cout << "erasing prize effect";
            undoEffect(prizeEffects[i].prizeEffectType);
            prizeEffects.erase(prizeEffects.begin() + i);
        }
    }

    // if (effect.timeOfEffectApplying > regularTimeOfPrizeEffect)
    // {
    //     undoEffect(effect.prizeEffectType);
    //     prizeEffects.erase(std::remove(prizeEffects.begin(), prizeEffects.end(), effect), prizeEffects.end());
    //     break;
    // }
}

bool shouldLeaveGame()
{
    return (gameState == quit ||
            gameState == menu_screen ||
            gameState == start_game);
}

void playGame(sf::RenderWindow &window)
{
    resetGlobalVars();
    getBestScores();
    background.setTexture(getBackgroundMainTexture());
    adjustPlatform();
    adjustBall();
    adjustGameLostModal();
    adjustSidebar();
    loadPrizesTextures();
    std::vector<Brick> bricks = createBricksVector_1level({x : 65, y : 60});
    sf::RectangleShape gameField = createGameField();
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        pollEvents(window, dt);
        updateBall(ballSpeed, dt, bricks);
        updatePrizes(dt);
        updatePrizeEffects(dt);
        redrawFrame(window, bricks, gameField);
        if (shouldLeaveGame())
        {
            handleScores();
            return;
        }
    }
}

//sf::Keyboard::isKeyPressed(sf::Keyboard::Space) : isGameStarted = true;