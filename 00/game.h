struct Brick
{
    sf::Sprite sprite;
    sf::Vector2f position;
    bool isDestroyed;
};

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
const sf::Vector2f gameFieldPosition = {x : 50, y : 50};
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

sf::Texture platformTexture;
sf::Sprite platform;
sf::Texture brickTexture;
sf::Sprite brick;
sf::Texture ballTexture;
sf::Sprite ball;
int ballXdir;
int ballYdir;
int ballSpeed;
bool isPaused;
int lives;
sf::Font font;
sf::Text levelLostMsgText;
sf::Text okText;
sf::Text cancelText;
std::string levelLostMsg = "YOU HAVE LOST THE LEVEL.\nWOULD YOU LIKE TO REPEAT ONE?";
std::string ok = "OK";
std::string cancel = "Cancel";
sf::RectangleShape levelLostModal;
int selectedModalItem = 1;

using namespace std;

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

Brick createBrick(sf::Color color, sf::Vector2f position)
{
    brick.setPosition(position);
    brick.setColor(color);
    Brick oneBrick;
    oneBrick.sprite = brick;
    oneBrick.position = position;
    oneBrick.isDestroyed = false;
    return oneBrick;
}

void handleCollisionWithBrick(sf::FloatRect brickBounds, sf::FloatRect ballBounds)
{
    const float ballCenter = ballBounds.left + BALL_SIZE / 2;
    if (
        (brickBounds.left <= ballCenter) &&
        (ballCenter <= (brickBounds.left + brickBounds.width)))
    {
        ballYdir = -1 * ballYdir;
    }
}

void handleCollisionWithPlatform(sf::FloatRect platformBounds, sf::FloatRect ballBounds)
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

void handleBallMiss()
{
    lives--;
    if (lives < 3)
    {
        gameState = level_lost_modal;
    }
    else
    {
        //gameState = ball_miss;
        resetPlatform();
        resetBall();
        isPaused = true;
        ballXdir = 1;
        ballYdir = -1;
    }
}

void updateBall(int &ballSpeed, float &dt, std::vector<Brick> &bricks)
{
    if (isPaused || gameState != playing)
        return;
    float angle = 45;
    const sf::Vector2f currentBallPos = ball.getPosition();
    const sf::FloatRect ballBounds = ball.getGlobalBounds();
    const sf::FloatRect platformBounds = platform.getGlobalBounds();
    for (int i = 0; i < bricks.size(); i++)
    {
        //const sf::FloatRect brickBounds = bricks[i].sprite.getGlobalBounds();
        sf::Sprite brick = bricks[i].sprite;
        const sf::FloatRect brickBounds = brick.getGlobalBounds();
        if (ballBounds.intersects(brickBounds))
        {
            handleCollisionWithBrick(brickBounds, ballBounds);
            bricks.erase(bricks.begin() + i);
        }
    }

    if (ballBounds.intersects(platformBounds))
    {
        handleCollisionWithPlatform(platformBounds, ballBounds);
    }

    const float S = ballSpeed * dt;
    sf::Vector2f newLocalCoords = toEuclidean(S, angle);
    newLocalCoords.x = ballXdir * newLocalCoords.x;
    newLocalCoords.y = ballYdir * newLocalCoords.y;
    sf::Vector2f newGlobalCoords = currentBallPos + newLocalCoords;

    if (newGlobalCoords.x >= RIGHT_EDGE - BALL_SIZE)
    {
        ballXdir = -1 * abs(ballXdir);
    }
    if (newGlobalCoords.x <= LEFT_EDGE + BALL_SIZE)
    {
        ballXdir = 1 * abs(ballXdir);
    }
    if (newGlobalCoords.y <= TOP)
    {
        ballYdir = 1 * abs(ballYdir);
    }
    if (newGlobalCoords.y >= BOTTOM)
    {
        handleBallMiss();
        return;
    }
    ball.setPosition(newGlobalCoords);
}

void updatePlatform(sf::Keyboard::Key &key, float &dt)
{
    // if (isPaused || gameState == level_lost_modal)
    //     return;
    const float speed = 1500;
    sf::Vector2f platformPosition = platform.getPosition();
    if (key == sf::Keyboard::Left)
    {
        if (platformPosition.x > gameFieldPosition.x)
        {
            platform.move(-1 * speed * dt, 0);
        }
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
            okText.setStyle(sf::Text::Bold | sf::Text::Underlined);
            cancelText.setStyle(sf::Text::Regular);
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
            okText.setStyle(sf::Text::Regular);
            cancelText.setStyle(sf::Text::Bold | sf::Text::Underlined);
        }
    }
}

void handleReturnKeyPressed()
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
        handleReturnKeyPressed();
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
        if (!bricks[i].isDestroyed)
        {
            window.draw(bricks[i].sprite);
        }
    }
}

void drawLevelLostModal(sf::RenderWindow &window)
{
    window.draw(levelLostModal);
    window.draw(levelLostMsgText);
    window.draw(okText);
    window.draw(cancelText);
}

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, std::vector<Brick> bricks, sf::RectangleShape field)
{
    window.clear(sf::Color::White);
    window.draw(field);
    drawBricks(window, bricks);
    window.draw(platform);
    window.draw(ball);
    if (gameState == level_lost_modal)
    {
        drawLevelLostModal(window);
    }
    window.display();
}

std::vector<Brick> createBricksVector_1level(sf::Vector2f startPosition)
{
    brickTexture.loadFromFile("00/images/brick.png");
    brick.setTexture(brickTexture);
    std::vector<Brick> bricks;
    int k = 0;
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            bricks.push_back(createBrick(sf::Color(250, 107, 235), {x : xStart, y : yStart}));
            k++;
            xStart = xStart + 45;
        }
        xStart = startPosition.x;
        yStart = yStart + 25;
    }
    return bricks;
}

sf::RectangleShape createGameField()
{
    sf::RectangleShape gameField;
    gameField.setSize({x : GAME_FIELD_WIDTH, y : GAME_FIELD_HEIGHT});
    gameField.setOutlineColor(sf::Color(76, 5, 73));
    gameField.setOutlineThickness(gameFieldOutlineThickness);
    gameField.setPosition(gameFieldPosition);
    gameField.setFillColor(sf::Color(191, 250, 107));
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

void adjustPlatform()
{
    platformTexture.loadFromFile("00/images/platform.png");
    platform.setTexture(platformTexture);
    sf::Vector2f platformSize(PLATFORM_WIDTH, PLATFORM_HEIGHT);
    platform.setScale(
        platformSize.x / platform.getLocalBounds().width,
        platformSize.y / platform.getLocalBounds().height);
    resetPlatform();
}

void adjustBall()
{
    ballTexture.loadFromFile("00/images/ball.png");
    ball.setTexture(ballTexture);
    ball.setScale(
        BALL_SIZE / ball.getLocalBounds().width,
        BALL_SIZE / ball.getLocalBounds().height);
    ballXdir = 1;
    ballYdir = -1;
    ballSpeed = 100;
    resetBall();
}

void adjustGameLostModal()
{
    font.loadFromFile("00/arial.ttf");
    levelLostModal.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    levelLostModal.setPosition(0, 0);
    levelLostModal.setFillColor(sf::Color(0, 0, 0, 200));

    levelLostMsgText = sf::Text();
    levelLostMsgText.setFillColor(sf::Color::Red);
    levelLostMsgText.setStyle(sf::Text::Bold);
    levelLostMsgText.setCharacterSize(25);
    levelLostMsgText.setString(levelLostMsg);
    levelLostMsgText.setPosition({x : WINDOW_WIDTH / 4, y : WINDOW_HEIGHT / 3});

    okText = sf::Text();
    okText.setFillColor(sf::Color::Red);
    okText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    okText.setCharacterSize(25);
    okText.setString(ok);
    okText.setPosition({x : WINDOW_WIDTH * 1 / 3, y : WINDOW_HEIGHT / 2});

    cancelText = sf::Text();
    cancelText.setFillColor(sf::Color::Red);
    cancelText.setStyle(sf::Text::Regular);
    cancelText.setCharacterSize(25);
    cancelText.setString(cancel);
    cancelText.setPosition({x : WINDOW_WIDTH * 2 / 3, y : WINDOW_HEIGHT / 2});

    levelLostMsgText.setFont(font);
    okText.setFont(font);
    cancelText.setFont(font);
}

void resetGlobalVars()
{
    isPaused = true;
    int lives = 3;
    int selectedModalItem = 1;
    gameState = playing;
}

void playGame(sf::RenderWindow &window, std::string playerName)
{
    adjustPlatform();
    adjustBall();
    adjustGameLostModal();

    resetGlobalVars();
    std::vector<Brick> bricks = createBricksVector_1level({x : 65, y : 60});
    sf::RectangleShape gameField = createGameField();
    sf::Clock clock;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        pollEvents(window, dt);
        updateBall(ballSpeed, dt, bricks);
        redrawFrame(window, bricks, gameField);

        if (
            gameState == quit ||
            gameState == menu_screen ||
            gameState == start_game)
        {
            return;
        }
    }
}

//sf::Keyboard::isKeyPressed(sf::Keyboard::Space) : isGameStarted = true;