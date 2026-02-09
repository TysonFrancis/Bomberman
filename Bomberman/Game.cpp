#include "Game.h"
#include <iostream>

Game::Game() : title(animations.getTitle()),                    // Load title sprite
    background(animations.getBackground()),                     // Load background sprite
    bomber(animations.getEntities()),                           // Load bomber sprite
    windowWidth(1984), windowHeight(832),                       // Define window size parameters
    window(sf::VideoMode({ windowWidth, windowHeight }),        // Create window with title and size
        "Bomberman", sf::Style::Titlebar | sf::Style::Close),
	state(GameState::Title), frame(0),                          // Set state to title screen and frame counter to 0

    enemy1(animations.getEntities(), 1), enemy2(animations.getEntities(), 2),        // Temporary player objects to test sprites and such
	enemy3(animations.getEntities(), 6)
{
    //Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(60);

	//Set title sprite on right texture, scale to fit and position in middle of window
    title.getSprite().setTextureRect(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.getSprite().setScale({ 3.5f, 3.5f });
    title.getSprite().setOrigin({ 128.f, 120.f });
    title.getSprite().setPosition({ windowWidth / 2.f, windowHeight / 2.f });

    //Set background sprite on right texture and scale to fit window
    background.getSprite().setTextureRect(sf::IntRect({ 0, 0 }, { 496, 208 }));
    background.getSprite().setScale({ 4.f, 4.f });

    //Set bomber sprite on starting texture and scale to see better
    bomber.getSprite().setTextureRect(sf::IntRect({ 64, 0 }, { 16, 16 }));
    bomber.getSprite().setScale({ 4.f, 4.f });
    bomber.getSprite().setPosition({ 64.f, 64.f });

    bomber.getAnimation().setScale({ 16.f, 16.f });
    

    bomber.getAnimation().setPosition({ 0.f, 0.f });
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
        if (event->is<sf::Event::Closed>() ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
            window.close();
    }

	if (state == GameState::Title &&    // If on title screen and enter is pressed, start game
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
        state = GameState::Playing;
}

// Sprite updater, calls each sprites update method
// with cuurent frame and increments frame counter
void Game::update()
{
    if(state != GameState::Playing)
		return;

	bomber.update();

	enemy1.update();

    // Automatic death animation testing if bomber collides with enemies
    if(enemy1.getBounds().findIntersection(bomber.getBounds()))
		enemy1.die();

	enemy2.update();

    if (enemy2.getBounds().findIntersection(bomber.getBounds()))
        enemy2.die();

    enemy3.update();

    if (enemy3.getBounds().findIntersection(bomber.getBounds()))
        enemy3.die();

        player.tick();

            window.clear();
            window.draw(bomber);
            window.display();

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