#include "Game.h"
#include <iostream>
#include "time.h"

using sf::Keyboard::isKeyPressed;
using sf::Keyboard::Scancode;
using namespace Constants;

Game::Game() : title(animations.getTitle()),                    // Load title sprite
    background(animations.getBackground()),                     // Load background sprite
    bomber(animations.getEntities(), pods),                     // Load bomber sprite
    window(sf::VideoMode({ _windowWidth, _windowHeight }),      // Create window with title and size
        "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    state(GameState::Title), frame(0)                           // Set state to title screen and frame count to 0 
{
    srand(time(NULL));

    // Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(2);

    // Set title sprite on right texture, scale to fit and position in middle of window
    title.setTexture(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.setOrigin({ 128.f, 120.f });
    title.setScale({ _scale * 0.875f, _scale * 0.875f}); // Best ratio fit for title screen
    title.setPosition({ _windowWidth / 2.f, _windowHeight / 2.f });

    // Set background sprite on right texture and scale to fit window
    background.setTexture(sf::IntRect({ 0, 0 }, { 496, 208 }));
    background.setScale({ _scale, _scale });

    // Set bomber sprite on starting texture and scale to see better
    bomber.setTexture(sf::IntRect({ 64, 0 }, { _tileSize, _tileSize }));
    bomber.setOrigin({ 8.f, 8.f });
    bomber.setScale({ _scale, _scale });
    bomber.setPosition({ _scaledTile * 1.5, _scaledTile * 1.5 });

    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Ballom));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Dahl));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Doria));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Minvo));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Onil));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Ovape));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Pass));
    enemies.push_back(Enemy(animations.getEntities(), pods, Enemy::Type::Pontan));

    for (Enemy& enemy : enemies)
    {
        enemy.setTexture(sf::IntRect({ 0, 224 }, { _tileSize, _tileSize }));
        enemy.setScale({ _scale, _scale });
        enemy.setPosition(sf::Vector2f(rand() % (((_cols - 2) * _scaledTile) - (5 * _scaledTile + 1)) + (5 * _scaledTile), rand() % (((_rows - 2) * _scaledTile) - (5 * _scaledTile + 1)) + (5 * _scaledTile)));
    }

    // Create pod system of walls and border
    for (int row = 0; row < _rows; row++)
        for (int col = 0; col < _cols; col++)
        {
            pods[row][col] = Pod(animations.getEntities(), // To pass entity textures
                sf::RectangleShape({_scaledTile, _scaledTile}),
                col * _scaledTile, row * _scaledTile);

			bool isInnerWall = col % 2 == 0 && row % 2 == 0;
			bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;
            bool isSoft = (rand() % 4 == 0 && (row > 2 || col > 2)); // Can't spawn in top 2 x 2 by player

            // Tile*'s deleted in pod desturctor, so no memory leak
            if (isInnerWall || isBorder)    // Set border
				pods[row][col].setTile(new HardWall);
            else if (isSoft)                // Set breakable blocks
                pods[row][col].setTile(new SoftWall);           
            else                            // Set inner
                pods[row][col].setTile(nullptr); // Set to nullptr to not display a texture later
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
}

// Handles window events like game starting and closing
void Game::events()
{
    while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>() || isKeyPressed(Scancode::Escape))
            closeGame();

    // If on title screen and enter is pressed, start game
    if (state == GameState::Title && isKeyPressed(Scancode::Enter))
        startRound();
}

// Sprite updater, calls each sprites update method
// with current frame and increments frame counter
void Game::update()
{
    // is it expensive to check this every frame?
    switch (state)
    {
    case GameState::RoundStart:

    bomber.update();

    for (Enemy& enemy : enemies)
        enemy.update();
    
    for (int row = 0; row < _rows; row++)
        for (int col = 0; col < _cols; col++)
            pods[row][col].update();

        frame++;
    }
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

        // I think that drawing pods is permanent the way
        // I've structured it so far, might need to refactor
        for (int row = 0; row < _rows; row++)
            for (int col = 0; col < _cols; col++)
                window.draw(pods[row][col].getShape());

        window.draw(bomber.getSprite());

        for (Enemy& enemy : enemies)
            window.draw(enemy.getSprite());
    }

    window.display();
}

void Game::startRound()
{
    state = GameState::RoundStart;
    audio.getRoundStart().play(); // Play silly music

    // There's a problem with the audio not playing fully
    // I think the issue is with the file itself idk I'll fix it later

    // Wait for music to finish
    while (audio.getRoundStart().getStatus() != sf::SoundSource::Status::Stopped)
    {
        window.clear();
        window.display(); // So it doesn't hang lol
    }

    state = GameState::Playing;
}

// Called when window is closed, used to
// ensure necessary things are destructed
// Currently nothing to destruct, but will be in future
void Game::closeGame() { window.close(); }