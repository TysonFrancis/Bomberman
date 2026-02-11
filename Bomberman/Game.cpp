#include "Game.h"
#include <iostream>

using namespace sf::Keyboard;

Game::Game() : title(animations.getTitle()),                // Load title sprite
background(animations.getBackground()),                     // Load background sprite
bomber(animations.getEntities()),                           // Load bomber sprite
windowWidth(750), windowHeight(750),                        // Define window size parameters, 1984 x 832 original
window(sf::VideoMode({ windowWidth, windowHeight }),        // Create window with title and size
    "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    state(GameState::Title), frame(0)                       // Set state to title screen and frame counter to 0
{
    //Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(60);

    //Set title sprite on right texture, scale to fit and position in middle of window
    title.setTexture(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.setScale({ 3.5f, 3.5f });
    title.setOrigin({ 128.f, 120.f });
    title.setPosition({ windowWidth / 2.f, windowHeight / 2.f });

    //Set background sprite on right texture and scale to fit window
    background.setTexture(sf::IntRect({ 0, 0 }, { 496, 208 }));
    background.setScale({ 4.f, 4.f });

    //Set bomber sprite on starting texture and scale to see better
    bomber.setTexture(sf::IntRect({ 64, 0 }, { 16, 16 }));
    bomber.setScale({ 4.f, 4.f });
    bomber.setPosition({ 64.f, 64.f });

    // ********** TYSON LOGIC STUFF ********** START ********** //
    player.setRadius(20);
    player.setOrigin({ 20, 20 });
    player.setPosition({ 20, 20 });

    // Regualar tiles creation
    for (int i = 0; i < num1; i++)
    {
        sf::RectangleShape square({ 50,50 });
        if (i % (width + x) < width)
        {
            square.setPosition(sf::Vector2f((((i - (x * (i / (x + width)))) % width) * 50), (((i - (x * (i / (x + width)))) / width) * 100)));
        }
        else
        {
            square.setPosition(sf::Vector2f((((i - (width * (i / (x + width))) - width) % x) * 100), (i - (width * (i / (x + width))) - width) / x * 100 + 50));
        }

        Pod temp(square, (int)(square.getPosition().x) % 50, (int)(square.getPosition().y) % 50);

        temp.filled = (rand() % 6 == 0);
        pods.push_back(temp);
    }

    // Unbreakable walls creation
    for (int i = 0; i < num2; i++)
    {
        sf::RectangleShape square({ 50,50 });
        square.setFillColor(sf::Color(255, 0, 0));
        square.setPosition(sf::Vector2f(i % (x - 1) * 100 + 50, i / (x - 1) * 100 + 50));
        Pod temp(square, 0, 0);
        walls.push_back(temp);
    }
    // ********** END ********** //
}

// Holds main game loop, all actions passed
// to supplementary methods for cleanliness
void Game::run()
{
    /*while (window.isOpen())
    {
        events();
        update();
        render();
    }*/

    // ********** TYSON LOGIC STUFF ********** START ********** //
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

    // ********** END ********** //
}

// Handles window events like game starting and closing
void Game::events()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>() || isKeyPressed(Scancode::Escape))
            window.close();
    }

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

    frame++;
}

// Handles all drawing and window render things
void Game::render()
{
    window.clear();

    if (state == GameState::Title)            // If not started, draw title
        window.draw(title.getSprite());
    else                                      // Else, draw other sprites
    {
        window.draw(background.getSprite());
        window.draw(bomber.getSprite());
    }

    window.display();
}