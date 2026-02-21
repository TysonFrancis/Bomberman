#include "Game.h"
#include <iostream>

using namespace sf::Keyboard;
using namespace Constants;

Game::Game() : title(animations.getTitle()),                    // Load title sprite
    background(animations.getBackground()),                     // Load background sprite
    bomber(animations.getEntities(), pods),                     // Load bomber sprite
    window(sf::VideoMode({ _windowWidth, _windowHeight }),      // Create window with title and size
        "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    state(GameState::Title), frame(0)                           // Set state to title screen and frame count to 0 
{
    //Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(60);

    //Set title sprite on right texture, scale to fit and position in middle of window
    title.setTexture(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.setOrigin({ 128.f, 120.f });
    title.setScale({ 0.875f * _scale, 0.875f * _scale}); // Best ratio fit for title screen
    title.setPosition({ _windowWidth / 2.f, _windowHeight / 2.f });

    //Set background sprite on right texture and scale to fit window
    background.setTexture(sf::IntRect({ 0, 0 }, { 496, 208 }));
    background.setScale({ _scale, _scale });

    //Set bomber sprite on starting texture and scale to see better
    bomber.setTexture(sf::IntRect({ 64, 0 }, { 16, 16 }));
    bomber.setOrigin({ 8.f, 8.f });
    bomber.setScale({ _scale, _scale });
    bomber.setPosition({ _scaledTile + 8.f * _scale, _scaledTile + 8.f * _scale });

    // Create pod system of walls and border
    for (int row = 0; row < _rows; row++)
        for (int col = 0; col < 31; col++)
        {
            pods[row][col] = Pod(sf::RectangleShape({ _scaledTile, _scaledTile }), col * _scaledTile, row * _scaledTile);

			bool isInnerWall = col % 2 == 0 && row % 2 == 0;
			bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;

            if (isInnerWall || isBorder)    // Set border
            {
				pods[row][col].setTile(new HardWall);           // Tile* deleted in pod desturctor, so no memory leak
                pods[row][col].setColor(sf::Color(125, 125, 255));
            }

            else                            // Set inner
                pods[row][col].setColor(sf::Color(125, 125, 125));
        }
}

// Holds main game loop, all actions passed
// to supplementary methods for cleanliness
void Game::run()
{
    while (window.isOpen())
    {
        events();
        update();
        render();
    }

    /* ********** TYSON LOGIC STUFF ********** START ********** //
    while (window.isOpen())
    {
        events();
        //Movement. Also collision
        //If player is colliding with a pod that is filled, it will stop player movement. Only checks the pod directly next to or above so it will
        //not stop if a player places a bomb. Issue is that the pod above and below are hard to check as they are a different distance away from playindex
        //I will work on a function that returns distance above and below
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && player.getPosition().x > 20)
        {
            player.move(sf::Vector2f(-2, 0));
            if (playindex > 0)
            {
                if (pods.at(playindex - (1)).shape.getGlobalBounds().findIntersection(player.getGlobalBounds()).has_value() && pods.at(playindex - 1).filled)
                    player.move(sf::Vector2f(2, 0));
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && player.getPosition().x < width * 50 - 20)
        {
            player.move(sf::Vector2f(2, 0));
            if (playindex < num1)
            {
                if (pods.at(playindex + (1)).shape.getGlobalBounds().findIntersection(player.getGlobalBounds()).has_value() && pods.at(playindex + 1).filled)
                    player.move(sf::Vector2f(-2, 0));
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && player.getPosition().y < depth * 50 - 20)
        {
            player.move(sf::Vector2f(0, 2));
            if (playindex < num1 - width)
            {
                if (pods.at(playindex + (width + x)).shape.getGlobalBounds().findIntersection(player.getGlobalBounds()).has_value() && pods.at(playindex).filled)
                    player.move(sf::Vector2f(0, -2));
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && player.getPosition().y > 20)
        {
            player.move(sf::Vector2f(0, -2));
            if (playindex > width - x)
            {
                if (pods.at(playindex - (width + x)).shape.getGlobalBounds().findIntersection(player.getGlobalBounds()).has_value() && pods.at(playindex - (width + x)).filled)
                    player.move(sf::Vector2f(0, 2));
            }
        }
        //Place a bomb
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            if (pods.at(playindex).filled == false)
                pods.at(playindex).fill();
        }
        window.clear();
        //If a pod is filled, draw it and make sure player does not collide with it
        for (int i = 0; i < num1; i++)
        {
            if (pods.at(i).filled)
            {
                window.draw(pods.at(i).shape);
            }
            if (pods.at(i).shape.getGlobalBounds().contains(player.getPosition()))
            {
                playindex = i;
            }

        }

        //Make sure player isn't colliding with a wall, also draw the wall bc no background
        for (int i = 0; i < num2; i++)
        {
            window.draw(walls.at(i).shape);
            if (player.getGlobalBounds().findIntersection((walls.at(i).shape.getGlobalBounds())).has_value())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                {
                    player.move(sf::Vector2f(2, 0));
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                {
                    player.move(sf::Vector2f(-2, 0));
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                {
                    player.move(sf::Vector2f(0, -2));
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    player.move(sf::Vector2f(0, 2));
                }
            }
        }
        window.draw(player);
        std::cout << playindex;
        window.display();
    }

    // ********** END ********** //*/
}

// Handles window events like game starting and closing
void Game::events()
{
    while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>() || isKeyPressed(Scancode::Escape))
            closeGame();

    // If on title screen and enter is pressed, start game
    if (state == GameState::Title && isKeyPressed(Scancode::Enter))
        state = GameState::Playing;
}

// Sprite updater, calls each sprites update method
// with current frame and increments frame counter
void Game::update()
{
    if (state != GameState::Playing)
        return;

    bomber.update();
    std::cout << bomber;

    frame++;
}

// Handles all drawing and window render things.
void Game::render()
{
    window.clear();
    if (state == GameState::Title)            // If not started, draw title
        window.draw(title.getSprite());
    else                                      // Else, draw other sprites
    {
        window.draw(background.getSprite());

		//* Displays pods for testing
        for (int row = 0; row < _rows; row++)
            for (int col = 0; col < _cols; col++)
                window.draw(pods[row][col].shape);
		//*/

        window.draw(bomber.getSprite());
    }

    window.display();
}

// Called when window is closed, used to
// ensure necessary things are destructed
// Currently nothing to destruct, but will be in future
void Game::closeGame()
{
    window.close();
}