#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <SFML/Window.hpp>
#include <vector>
#include <conio.h>
#include <string>
#include "assets.cpp"
#include "store.cpp"
#include "globalVars.cpp"
#include "game.h"
#include "splashScreen.h"
#include "menu.h"
#include "inputName.h"

int main()
{
    loadAssets();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "arkanoid game", sf::Style::Default, settings);

    //splashSreen(window);
    bool game = true;
    while (game)
    {
        switch (gameState)
        {
        case menu_screen:
            menu(window);
            break;
        case input_name:
            getPlayerName(window);
            break;
        case start_game:
            playGame(window);
            break;
        case quit:
            game = !game;
            window.close();
        }
    }
}

//узнать размер sprite
// const sf::Vector2f platformSize(
//     platformSprite.getTexture()->getSize().x * platformSprite.getScale().x,
//     platformSprite.getTexture()->getSize().y * platformSprite.getScale().y);