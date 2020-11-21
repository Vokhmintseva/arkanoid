#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <SFML/Window.hpp>

struct Brick
{
    sf::Sprite sprite;
    sf::Vector2f position;
    bool isDestroyed;
};

Brick createBrick(sf::Sprite brickSprite, sf::Color color, sf::Vector2f position)
{
    brickSprite.setPosition(position);
    brickSprite.setColor(color);
    Brick oneBrick;
    oneBrick.sprite = brickSprite;
    oneBrick.position = position;
    oneBrick.isDestroyed = false;
    return oneBrick;
}

//опрашивает и обрабатывает доступные события в цикле
void pollEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }
}

void drawBricks(sf::RenderWindow &window, Brick *bricks)
{
    int length = 30;
    
    for (int i = 0; i < length; i++)
    {
        window.draw(bricks[i].sprite);
    }
}

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, Brick *bricks, sf::RectangleShape field)
{
    window.clear(sf::Color::White);
    window.draw(field);
    drawBricks(window, bricks);
    window.display();
}

Brick *createBricksArray_1level(sf::Vector2f startPosition, sf::Sprite brickSprite)
{
    //const int NUMBER_OF_BRICKS = 30;
    Brick *bricks = new Brick[30];
    int k = 0;
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            bricks[k] = createBrick(brickSprite, (sf::Color(250, 107, 235)), {x : xStart, y : yStart});
            k++;
            xStart = xStart + 45;
        }
        xStart = startPosition.x;
        yStart = yStart + 25;
    }
    return bricks;
}

sf::RectangleShape createGameField(sf::Vector2f position, sf::Vector2f size)
{
    sf::RectangleShape gameField;
    gameField.setSize(size);
    gameField.setOutlineColor(sf::Color(76, 5, 73));
    gameField.setOutlineThickness(5);
    gameField.setPosition(position);
    gameField.setFillColor(sf::Color(191, 250, 107));
    return gameField;
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "arkanoid game", sf::Style::Default, settings);

    bool gameOver;
    bool gameWon;
    bool gameStarted;
    bool paused;

    sf::Texture brickTexture;
    brickTexture.loadFromFile("00/images/brick3.png");
    sf::Sprite brickSprite;
    brickSprite.setTexture(brickTexture);

    Brick *bricks = createBricksArray_1level({x : 55, y : 60}, brickSprite);

    sf::RectangleShape gameField = createGameField({x : 50, y : 50}, {x : 275, y : 300});

    while (window.isOpen())
    {
        pollEvents(window);
        // updatePointer(mousePosition, pointer);
        // updateCat(mousePosition, cat, clock);
        redrawFrame(window, bricks, gameField);
    }
}

//sf::Vector2f brickSize(40.0f, 25.0f);
// brickSprite.setScale(
//     brickSize.x / brickSprite.getLocalBounds().width,
//     brickSize.y / brickSprite.getLocalBounds().height);

//Brick *bricks = new Brick[NUMBER_OF_BRICKS];
//sf::Sprite brickSprite = createBrickSprite(brickSize);