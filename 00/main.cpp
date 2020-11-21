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

// Рисует и выводит один кадр
void redrawFrame(sf::RenderWindow &window, Brick *bricks)
{
    window.clear(sf::Color::White);
    //int length = sizeof(bricks);
    for (int i = 0; i < 30; i++)
    {
        window.draw(bricks[i].sprite);
        //std::cout << "drawing brick" << std::endl;
    }
    //drawBricks();
    window.display();
}

Brick *createBricksArray_1level(sf::Vector2f startPosition, sf::Sprite brickSprite)
{
    const int NUMBER_OF_BRICKS = 30;
    Brick *bricks = new Brick[NUMBER_OF_BRICKS];
    int k = 0;
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            bricks[k] = createBrick(brickSprite, (sf::Color(0xCF, 0xEF, 0x28)), {x : xStart, y : yStart});
            k++;
            xStart = xStart + 45;
        }
        xStart = 0;
        yStart = yStart + 30;
    }
    return bricks;
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

    sf::Texture brickTexture;
    brickTexture.loadFromFile("00/images/brick2.png");
    sf::Sprite brickSprite;
    brickSprite.setTexture(brickTexture);

    //Brick bricks[NUMBER_OF_BRICKS];

    bool gameOver;
    bool gameWon;
    bool gameStarted;
    bool paused;
    sf::Vector2f brickSize(40.0f, 25.0f);
    // int k = 0;
    // float xStart = 0;
    // float yStart = 0;
    // for (int i = 0; i < 5; i++)
    // {
    //     for (int j = 0; j < 6; j++)
    //     {
    //         bricks[k] = createBrick(brickSprite, (sf::Color(0xCF, 0xEF, 0x28)), {x : xStart, y : yStart});
    //         k++;
    //         xStart = xStart + 45;
    //     }
    //     xStart = 0;
    //     yStart = yStart + 30;
    // }
    //sf::Vector2f pos(20, 40);

    Brick *bricks = createBricksArray_1level({x : 20, y : 40}, brickSprite);
    while (window.isOpen())
    {
        pollEvents(window);
        // updatePointer(mousePosition, pointer);
        // updateCat(mousePosition, cat, clock);
        redrawFrame(window, bricks);
    }
}

//sf::Vector2f brickSize(40.0f, 25.0f);
// brickSprite.setScale(
//     brickSize.x / brickSprite.getLocalBounds().width,
//     brickSize.y / brickSprite.getLocalBounds().height);

//Brick *bricks = new Brick[NUMBER_OF_BRICKS];
//sf::Sprite brickSprite = createBrickSprite(brickSize);