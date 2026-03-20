#include "Game.h"
#include <iostream>

using namespace Constants;
using std::cout;
using std::endl;

Game::Game() : title(animations.getTitle()), endTitle(title),   // Load title sprites
background(animations.getBackground()),                         // Load background sprite
bomber(animations.getEntities(), pods, bombs, explosions),      // Load bomber entity
window(sf::VideoMode({ _windowWidth, _windowHeight }),          // Create window with title and size
    "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    gameTick(0), score(0), streak(0), combo(0), enemyType(0)    // Set misc values to 0
{
    srand(time(NULL));

    // Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(_fps);

    // Set title sprite on right texture, scale to fit and position in middle of window
    title.setTextureRect(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.setOrigin({ 128.f, 120.f });
    title.setScale({ _scale * 0.875f, _scale * 0.875f });        // Best ratio fit for title screen
    title.setPosition({ _windowWidth / 2.f, _windowHeight / 2.f });

    // Set to other title screen with same everything else as title
    endTitle = title;
    endTitle.setTextureRect(sf::IntRect({ 256, 0 }, { 256, 240 }));

    // Set background sprite on right texture and scale to fit window
    background.setTextureRect(sf::IntRect({ 0, 0 }, { 496, 208 }));
    background.setScale({ _scale, _scale });

    // Create pod system of walls and border
    for (int row = 0; row < _rows; row++)
        for (int col = 0; col < _cols; col++)
        {
            bool isInnerWall = col % 2 == 0 && row % 2 == 0;
            bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;
            bool isSoft = (rand() % 4 == 0) && (row > 2 || col > 2); // Can't spawn in top 2 x 2 by player

            if (isInnerWall || isBorder)
            {
                pods[row][col].isFilled = true;
                pods[row][col].isHard = true;
            }
            else if (isSoft)
            {
                pods[row][col].isFilled = true;
                pods[row][col].isSoft = true;
                //pods[row][col].isExit = true;
                softWalls.push_back(SoftWall(animations.getEntities(), pods, col, row));
            }
        }

    // Make 5 enemies and put them in positions that are empty
    for (int i = 0; i < 5; i++)
    {
        Enemy enemy(animations.getEntities(), pods, Enemy::Type::Ballom, bomber);
        int x, y;

        do
        {
            x = rand() % 29 + 2;
            y = rand() % 11 + 2;
        } while (pods[y][x].isFilled);

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
    // Close game if window is closed or escape key pressed
    while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>() ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
            closeGame();

    // If on title screen and enter is pressed, start game
    if (s_gameState == GameState::Title &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
        startRound();
}

// Sprite updater, calls each sprites update method
// with current frame and increments frame counter
void Game::update()
{
    switch (s_gameState)
    {
    case(GameState::Playing):
        bomber.update();

        for (size_t i = 0; i < enemies.size(); i++)
        {
            enemies[i].update();

            if (enemies[i].intersects(bomber))
                bomber.die();

            streak -= 1;
            if (enemies[i].getState() == Entity::State::Dead)
            {
                //Check for combo kills
                if (streak > 0)
                    combo += 1;
                else combo = 1;

                enemyType = static_cast<int>(enemies[i].getType());

                switch (enemyType)//Update score when enemy dies
                {
                case 0: case 1: score += (enemyType + 1) * 100 * combo; break;
                case 2: case 3: score += (enemyType - 1) * 200 * combo; break;
                case 4: case 5: score += (enemyType - 3) * 1000 * combo; break;
                case 6: case 7: score += (enemyType - 5) * 2000 * combo; break;
                }

                streak = 20; //Waits 20 frames to check for other deaths

                //Display score after death using points

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

            if (explosions[i].intersects(bomber))
                bomber.die();

            for (Enemy& enemy : enemies)
                if (explosions[i].intersects(enemy))
                    enemy.die();

            for (Bomb& bomb : bombs)
                if (explosions[i].intersects(bomb) && !bomb.getWillExplode())
					bomb.delay();//Explodes in 3 frames

            if (explosions[i].getState() == Entity::State::Dead)
            {
                explosions.erase(explosions.begin() + i);
                i--;
            }
        }
		//std::cout << "\nExplosions: " << explosions.size();

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
    case(GameState::RoundStart):               break;
    case(GameState::GameOver):    gameTick++;  break;
    }
}

// Handles all drawing and window render things
void Game::render()
{
    window.clear();

    switch (s_gameState)
    {
    case(GameState::Playing):
        window.draw(background);

        for (SoftWall& wall : softWalls)
            window.draw(wall);

        for (Bomb& bomb : bombs)
            window.draw(bomb);

        for (Explosion& explosion : explosions)
            window.draw(explosion);

        window.draw(bomber);

        for (Enemy& enemy : enemies)
            window.draw(enemy);

        break;

    case(GameState::Title):       window.draw(title);          break;
    case(GameState::RoundStart):  /* Draw current round??? */  break;
    case(GameState::GameOver):    window.draw(endTitle);       break;
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

    s_gameState = GameState::Playing;
}

// Called when window is closed, used to
// ensure necessary things are destructed
// Currently nothing to destruct, but will be in future
void Game::closeGame()
{
    window.close();
}