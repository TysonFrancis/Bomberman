#include "Game.h"
#include <iostream>

using namespace sf::Keyboard;

Game::Game() : title(animations.getTitle()),                // Load title sprite
background(animations.getBackground()),                     // Load background sprite
bomber(animations.getEntities()),                           // Load bomber sprite
windowWidth(1984), windowHeight(832),                       // Define window size parameters
window(sf::VideoMode({ windowWidth, windowHeight }),        // Create window with title and size
    "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    state(GameState::Title), frame(0),                      // Set state to title screen and frame counter to 0

    enemy1(animations.getEntities(), 1), enemy2(animations.getEntities(), 2),        // Temporary player objects to test sprites and such
    enemy3(animations.getEntities(), 6)
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

    // ********** TEMPORARY **********
    enemy1.setTexture(sf::IntRect({ 0, 240 }, { 16, 16 }));
    enemy1.setScale({ 4.f, 4.f });
    enemy1.setPosition({ 320.f, 256.f });

    enemy2.setTexture(sf::IntRect({ 0, 256 }, { 16, 16 }));
    enemy2.setScale({ 4.f, 4.f });
    enemy2.setPosition({ 576.f, 512.f });

    enemy3.setTexture(sf::IntRect({ 0, 320 }, { 16, 16 }));
    enemy3.setScale({ 4.f, 4.f });
    enemy3.setPosition({ 832.f, 320.f });
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

    // Automatic death animation testing if bomber collides with enemies
    enemy1.update();
    if (enemy1.intersects(&bomber))
        enemy1.die();

    enemy2.update();
    if (enemy2.intersects(&bomber))
        enemy2.die();

    enemy3.update();
    if (enemy3.intersects(&bomber))
        enemy3.die();

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
        window.draw(enemy1.getSprite());
        window.draw(enemy2.getSprite());
        window.draw(enemy3.getSprite());
    }

    window.display();
}