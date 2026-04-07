#include "Game.h"
#include <iostream>

using namespace Constants;

using std::cout, std::endl;

Game::Game() : title(animations.getTitle()), endTitle(title),   // Load title sprites
background(animations.getBackground()),                     // Load background sprite
bomber(animations.getEntities(), pods, bombs, explosions),  // Load bomber entity
window(sf::VideoMode({ _windowWidth, _windowHeight }),      // Create window with title and size
    "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    gameTick(0), score(0), streak(0), combo(0), enemyType(0),   // Set misc values to 0
    stage(0), levelTransition(false), gameOver(false), bonus(false)
{
    srand((static_cast<unsigned>(time(nullptr))));

    // Start title music
    audio.getMusic("title").play();
    audio.getMusic("title").setVolume(50);
    audio.getMusic("title").setLooping(true);
    song = "title";

    // Set window icon and framerate
    window.setIcon(animations.getIcon());
    window.setFramerateLimit(_fps);

    // Set title sprite on right texture, scale to fit and position in middle of window
    title.setTextureRect(sf::IntRect({ 0, 0 }, { 256, 240 }));
    title.setOrigin({ 128.f, 120.f });
    title.setScale({ _scale * 0.875f, _scale * 0.875f });        // Best ratio fit for title screen
    title.setPosition(_centerScreen);

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

            if (isInnerWall || isBorder)
            {
                pods[row][col].isFilled = true;
                pods[row][col].isHard = true;
            }
        }

    level();
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
    // Close game if window is closed, escape key pressed, or on game over screen and enter pressed
    while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape) ||
            s_gameState == GameState::GameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
            closeGame();

    // If on title screen and enter is pressed, start game
    if (s_gameState == GameState::Title && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
    {
        audio.getMusic("title").stop();
        s_gameState = GameState::RoundStart;
    }

    if (gameTick == _bonusTimer && bonus)                       // End bonus stage after 30 seconds
        s_gameState = GameState::Transition;
    if (bonus)                                                  // Spawn enemies for bonus stage
    {
        if (stage < 40)                                              // If under 40 increment enemy type
            spawnEnemies(static_cast<Enemy::Type>(stage / 5));
        else                                                        // If 40 or above spawn Pontans
            spawnEnemies(Enemy::Type::Pontan);
    }
    if (gameTick >= _pontanTimer && !timerExpired)              // Spawn Pontans if past 200 seconds
    {
        timerExpired = true;
        spawnEnemies(Enemy::Type::Pontan);
    }
}

// Sprite updater, calls each sprites update method
// with current frame and increments frame counter
void Game::update()
{
    switch (s_gameState)
    {
    case(GameState::Playing):
        gameTick++;

        // Moved enemy spawning items into events method

        bomber.update();

        for (size_t i = 0; i < enemies.size(); i++)
        {
            enemies[i].update();

            if (enemies[i].intersects(bomber) && !bonus)
            {
                levelTransition = (bomber.getLives() > 1);
                bomber.die();

            }

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
                s_enemyCount--;
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

            // If bomber doesn't have fire shield and is colliding with explosion, die
            if (!bomber.hasFireShield())
                if (explosions[i].intersects(bomber) && !bonus)
                {
                    levelTransition = (bomber.getLives() > 1);
                    bomber.die();
                }

            // If explosion is colliding with enemy, kill enemy
            for (Enemy& enemy : enemies)
                if (explosions[i].intersects(enemy))
                    enemy.die();

            // If explosion is colliding with bomb, explode bomb after 3 frames
            for (Bomb& bomb : bombs)
                if (explosions[i].intersects(bomb) && !bomb.getWillExplode())
                    bomb.delay();       // Explodes in 3 frames

            // If explosion is colliding with powerup, spawn enemies and remove powerup
            if (powerUp && powerUp->getSprite().getGlobalBounds().
                findIntersection(explosions[i].getSprite().getGlobalBounds()))
            {
                spawnEnemies(getEnemyType());     // Spawn enemies if powerup is hit by explosion
                powerUp.reset();                  // Remove powerup if hit by explosion
            }

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

        if (powerUp && powerUp->getSprite().getGlobalBounds().
            findIntersection(bomber.getSprite().getGlobalBounds()))
        {
            powerUp->applyEffect(bomber);
            cout << *powerUp << "\n";
            powerUp.reset();
        }

        break;

    case(GameState::RoundStart):
        if (!levelTransition)       // To prevent restarting audio and text every frame while on round start screen
        {
            audio.playSound("roundStart");

            if (!bonus)
                textObjects.emplace_back(new Text("stage " + std::to_string(stage + 1), _centerScreen));
            else
                textObjects.emplace_back(new Text("bonus stage", _centerScreen));

            levelTransition = true;
        }

        // Wait until audio finishes
        if (audio.getStatus("roundStart") == sf::SoundSource::Status::Stopped)
        {
            for (Text* text : textObjects)      // Clean up text objects after round start audio finishes
                delete text;
            textObjects.clear();

            s_gameState = GameState::Playing;

            audio.getMusic("main").play();
            audio.getMusic("main").setLooping(true);
            song = "main";

            levelTransition = false;
            gameTick = 0;
        }

        break;

    case(GameState::Transition):
        audio.getMusic(song).stop();

        if (!levelTransition)       // To prevent restarting audio and text every frame while on round start screen
        {
            audio.playSound("stageClear");
            textObjects.emplace_back(new Text("stage clear", _centerScreen));
            levelTransition = true;

            if (!bonus)
                stage++;
            if (bomber.getLives() < 3)
                bomber.addLife();
            if (/*stage != 1 && */stage % 5 == 1 && !bonus)
            {
                bonus = true;
                clear();
            }
            else
                bonus = false;
        }

        // Wait until audio finishes
        if (audio.getStatus("stageClear") == sf::SoundSource::Status::Stopped)
        {
            for (auto* text : textObjects)
                delete text;
            textObjects.clear();

            if (!bonus)
                level();

            s_gameState = GameState::RoundStart;
            levelTransition = false;
        }

        break;

    case (GameState::GameOver):
        audio.getMusic(song).stop();

        if (!levelTransition)       // To prevent restarting audio and text every frame while on game over screen
        {
            audio.playSound("gameOver");
            textObjects.emplace_back(new Text("game over", _centerScreen));
            levelTransition = true;
        }

        // Wait until audio finishes
        if (audio.getStatus("gameOver") == sf::SoundSource::Status::Stopped)
        {
            for (Text* text : textObjects)      // Clean up text objects after game over audio finishes
                delete text;
            textObjects.clear();

            gameOver = true;        // For screen display after audio
        }
        break;
    }
}

// Handles all drawing and window rendering
void Game::render()
{
    window.clear();

    switch (s_gameState)
    {
    case(GameState::Playing):
        window.draw(background);

        if (powerUp)
            window.draw(powerUp->getSprite());

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

    case(GameState::GameOver):
        if (gameOver)
        {
            window.draw(endTitle);
            break;
        }
        [[fallthrough]];
    case(GameState::RoundStart):
    case(GameState::Transition):
        for (Text* text : textObjects)
            for (sf::Sprite* glyph : text->sprites)
                window.draw(*glyph);

        break;

    case(GameState::Title):     window.draw(title);     break;
    }

    window.display();
}

// Called when window is closed, used to
// ensure necessary things are closed
void Game::closeGame()
{
    window.close();
}


// Called after a powerup or exit is hit, the pontan
// timer expires, or on bonus stage, spawns enemies of
// given type and places them randomly on the map
void Game::spawnEnemies(Enemy::Type type)
{
    int size = enemies.size();          // Number of current enemies
    int copy = std::min(size, 10);      // Number of enemies needed to copy back to vector
    int spawn = 10 - copy;              // Number of new enemies needed to spawn

    if (timerExpired)                   // If timer expired for Pontans
    {
        std::vector<Enemy> newEnemies;
        newEnemies.reserve(10);

        for (int i = 0; i < copy; i++)      // For how ever many enemies are currently
        {                                   // alive, replace them with a new Pontan
            Enemy enemy(animations.getEntities(), pods, Enemy::Type::Pontan, bomber);
            enemy.setPosition(enemies[i].getX(), enemies[i].getY());

            newEnemies.emplace_back(enemy);     // Place new enemy in Pontan vector
        }

        for (int i = 0; i < spawn; i++)     // For how ever many new enemies to make,
        {                                   // place them randomly on the map
            Enemy enemy(animations.getEntities(), pods, Enemy::Type::Pontan, bomber);
            int x, y;

            do
            {
                x = rand() % (_cols - 1) + 1;
                y = rand() % (_rows - 1) + 1;
            } while (pods[y][x].isFilled);

            enemy.setPosition(x, y);
            newEnemies.emplace_back(enemy);     // Place new enemy in Pontan vector
        }

        enemies = std::move(newEnemies);    // Copy over all Pontans back into original enemies vector
    }

    else                                // Bonus stage or other spawns, just add them to the current vector without removing current enemies
    {
        for (int i = 0; i < spawn; i++)     // For how ever many new enemies to make,
        {                                   // place them randomly on the map
            Enemy enemy(animations.getEntities(), pods, type, bomber);
            int x, y;

            do
            {
                x = rand() % (_cols - 1) + 1;
                y = rand() % (_rows - 1) + 1;
            } while (pods[y][x].isFilled);

            enemy.setPosition(x, y);
            enemies.emplace_back(enemy);        // Place new enemy in original vector
        }
    }

    s_enemyCount = enemies.size();      // Re-initialize enemy count
}

void Game::clear()
{
    for (int row = 0; row < _rows; row++)
    {
        for (int col = 0; col < _cols; col++)
        {
            bool isInnerWall = col % 2 == 0 && row % 2 == 0;
            bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;
            if (!(isInnerWall || isBorder))
                pods[row][col].isBomb = pods[row][col].isExit = pods[row][col].isFilled = pods[row][col].isSoft = false;
        }
    }

    bomber.setPosition(1, 1);
    bomber.setTexture(64, 0);
    s_enemyCount = 0;
    enemies.clear();
    bombs.clear();
    softWalls.clear();
    explosions.clear();
    powerUp.reset();
    gameTick = 0;
}

void Game::level()
{
    cout << "level: " << stage + 1 << "\n";

    clear();

    int walls = stage * 2 + 54;
    int playable = _softPods;
    int power = powerupPresets[stage];

    for (int row = 0; row < _rows; row++)
    {
        for (int col = 0; col < _cols; col++)
        {
            if (walls == 0)
                break;
            bool isSoft = (rand() % (playable / walls) == 0) && (row > 2 || col > 2); // Can't spawn in top 2 x 2 by player
            bool isInnerWall = col % 2 == 0 && row % 2 == 0;
            bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;
            if (!(isInnerWall || isBorder))
            {
                if (isSoft)
                {
                    pods[row][col].isFilled = true;
                    pods[row][col].isSoft = true;
                    softWalls.emplace_back(SoftWall(animations.getEntities(), pods, col, row));
                    walls--;
                }
                playable--;
            }
        }
    }

    if (softWalls.size() == 0)
    {
        cout << "No soft walls to place exit and powerup on\n";
        return;
    }

    int exitIndex = rand() % softWalls.size();                                   // Get random index for exit and powerup in soft wall vector
    int powerUpIndex = rand() % softWalls.size();

    while (powerUpIndex == exitIndex)                                                 // If they are the same, get a new random
        powerUpIndex = rand() % softWalls.size();                                // index for powerup until they are different

    pods[softWalls[exitIndex].getY()][softWalls[exitIndex].getX()].isExit = true;     // Set selected pod to be exit
    powerUp.emplace(PowerUp(animations.getMisc(), pods,                  // Make a new powerup of random type at the selected powerup position
        static_cast<PowerUp::Type>(power), softWalls[powerUpIndex].getX(), softWalls[powerUpIndex].getY()));

    cout << "Exit set at: (" << softWalls[exitIndex].getX() << ", " << softWalls[exitIndex].getY() << "), "
        << "Powerup set at: (" << softWalls[powerUpIndex].getX() << ", " << softWalls[powerUpIndex].getY() << ")\n";

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < enemyPresets[stage % 50][k]; i++)
        {
            Enemy enemy(animations.getEntities(), pods, Enemy::Type(k), bomber);
            int x, y;

            do
            {
                x = rand() % (_cols - 3) + 3;
                y = rand() % (_rows - 3) + 3;
            } while (pods[y][x].isFilled);

            enemy.setPosition(x, y);
            enemies.emplace_back(enemy);
            s_enemyCount++;
        }
    }
}

Enemy::Type Game::getEnemyType() const
{
    if (!powerUp)
    {
        cout << "No powerup to get enemy type from\n";
        return Enemy::Type::Ballom;     // Default enemy type if no powerup, should never be used
    }

    switch (powerUp->getType())
    {
    case PowerUp::Type::ExtraBomb:	return Enemy::Type::Ballom;
    case PowerUp::Type::ExtraRange:	return Enemy::Type::Onil;
    case PowerUp::Type::Skate:		return Enemy::Type::Dahl;
    case PowerUp::Type::WallPhase:	return Enemy::Type::Minvo;
    case PowerUp::Type::Remote:		return Enemy::Type::Doria;
    case PowerUp::Type::BombPhase:	return Enemy::Type::Ovape;
    case PowerUp::Type::FireShield:	return Enemy::Type::Pass;
    case PowerUp::Type::Invincible:	return Enemy::Type::Pontan;
    }
}