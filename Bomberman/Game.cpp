#include "Game.h"
#include <iostream>
#include "time.h"

using namespace Constants;

using sf::Keyboard::isKeyPressed;
using sf::Keyboard::Scancode;

Game::Game() : title(animations.getTitle()),                    // Load title sprite
    background(animations.getBackground()),                     // Load background sprite
    bomber(animations.getEntities(), pods, bombs, explosions),  // Load bomber sprite
    window(sf::VideoMode({ _windowWidth, _windowHeight }),      // Create window with title and size
        "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    state(GameState::Title)                                     // Set state to title screen
{
    srand(time(NULL));

    // Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(_fps);

    // Set title sprite on right texture, scale to fit and position in middle of window
    title.setTextureRect(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.setOrigin({ 128.f, 120.f });
    title.setScale({ _scale * 0.875f, _scale * 0.875f});        // Best ratio fit for title screen
    title.setPosition({ _windowWidth / 2.f, _windowHeight / 2.f });

    // Set background sprite on right texture and scale to fit window
    background.setTextureRect(sf::IntRect({ 0, 0 }, { 496, 208 }));
    background.setScale({ _scale, _scale });

    // Create pod system of walls and border
    for (int row = 0; row < _rows; row++)
        for (int col = 0; col < _cols; col++)
        {
			bool isInnerWall = col % 2 == 0 && row % 2 == 0;
			bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;
            bool isSoft = (rand() % 4 == 0 && (row > 2 || col > 2)); // Can't spawn in top 2 x 2 by player

            if (isInnerWall || isBorder)
            {
                pods[row][col].filled = true;
                pods[row][col].isHard = true;
            }
            else if(isSoft)
            {
                pods[row][col].filled = true;
				pods[row][col].isSoft = true;
				softWalls.push_back(SoftWall(animations.getEntities(), pods, col, row));
            }
            else
				pods[row][col].filled = false;
        }

    // Make 5 enemies and put them in positions that are empty
    for (int i = 0; i < 5; i++)
    {
        Enemy enemy(animations.getEntities(), pods, Enemy::Type::Ballom,bomber);
        int x, y;

        do
        {
            x = rand() % 29 + 2;
            y = rand() % 11 + 2;
        } while (pods[y][x].filled);

        enemy.setPosition(sf::Vector2f(x * _scaledTile - _halfScaled, y * _scaledTile - _halfScaled));
		enemies.push_back(enemy);
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
	switch(state)
    {
	case(GameState::Playing):
        bomber.update();

        for (size_t i = 0; i < enemies.size(); i++)
        {
            enemies[i].update();

            if (enemies[i].intersects(bomber))
                bomber.die();

            if (enemies[i].getState() == Entity::State::Dead)
            {
                enemies.erase(enemies.begin() + i);
                i--;
            }
        }

        for (size_t i = 0; i < bombs.size(); i++)
        {
            bombs[i].update();

            if (bombs[i].getState() == Entity::State::Dead)
            {
                bombs.erase(bombs.begin() + i);
                i--;
            }
        }

        for (size_t i = 0; i < explosions.size(); i++)
        {
            explosions[i].update();

            /*if (explosions[i].intersects(bomber))
                bomber.die();*/

            for (Enemy& enemy : enemies)
                if(explosions[i].intersects(enemy))
                    enemy.die();

            for (Bomb& bomb : bombs)
                if (explosions[i].intersects(bomb))
                    bomb.explode();

            if (explosions[i].getState() == Entity::State::Dead)
            {
                explosions.erase(explosions.begin() + i);
                i--;
            }
        }

        for (size_t i = 0; i < softWalls.size(); i++)
        {
            softWalls[i].update();

            if (softWalls[i].getState() == Entity::State::Dead)
            {
                softWalls.erase(softWalls.begin() + i);
                i--;
            }
        }

        break;

    case(GameState::Title):
    case(GameState::RoundStart):
    case(GameState::GameOver): break;
    }
}

// Handles all drawing and window render things
void Game::render()
{
    window.clear();

    switch(state)
    {
    case(GameState::Playing):
        window.draw(background);

        for (Bomb& bomb : bombs)
            window.draw(bomb);

        for (Explosion& explosion : explosions)
            window.draw(explosion);

        window.draw(bomber);

        for (SoftWall& wall : softWalls)
            window.draw(wall);

        for (Enemy& enemy : enemies)
            window.draw(enemy);

        break;

    case(GameState::Title):         window.draw(title);             break;
	case(GameState::RoundStart):    /* Draw current round??? */
	case(GameState::GameOver):      /* Draw game over screen??? */  break;
    }

    window.display();
}

void Game::startRound()
{/*
    state = GameState::RoundStart;
    audio.getRoundStart().play(); // Play silly music

    // Wait for music to finish
    while (audio.getRoundStart().getStatus() != sf::SoundSource::Status::Stopped)
    {
        // Is this what you meant by should we have text on screen Emery???
        // I agree a blank screen with no text isn't great, and if you don't have
        // audio on you might think the game is loading slow. I'm going to disable the screen
        // clearing for now so it stays on title until we have something else to display. - Dylan

        //window.clear();   - - -   Commented out until something else to display
        //window.display(); // So it doesn't hang lol
    }*/
    
    state = GameState::Playing;
}

// Called when window is closed, used to
// ensure necessary things are destructed
// Currently nothing to destruct, but will be in future
void Game::closeGame()
{
    window.close();
}