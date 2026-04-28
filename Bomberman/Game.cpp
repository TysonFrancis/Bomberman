#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <cstdlib>      // For srand and rand
#include <ctime>        // For time
#include <algorithm>    // For std::min

using namespace Constants;
using std::cout, std::endl;

Game::Game() : background(animations.getBackground()),              // Load background sprite
    title(animations.getTitle()), endTitle(animations.getTitle()),  // Load title sprites
    bomber(animations.getEntities(), pods, bombs, explosions),      // Load bomber entity
    window(sf::VideoMode({ _windowWidth, _windowHeight }),          // Create window with title and size
        "Bomberman", sf::Style::Titlebar | sf::Style::Close),
    world(window.getDefaultView()), UI(window.getDefaultView()),    // Set view blocks
    panel(animations.getMisc(), animations.getEntities(),           // Load information panel
        animations.getTitle()),
    gameState(GameState::Title), gameTick(0), stage(0),             // Set misc values to defaults
    invincibilePlayerTicks(0), active(false),
    streak(0), combo(0), enemyType(0),
    levelTransition(false), levelTimerExpired(false),
    gameOver(false), bonus(false)
{
    // Seed random number table
    srand((static_cast<unsigned>(time(nullptr))));

    // Move the world view down 2 tiles
    world.move(sf::Vector2f(0, -2 * _scaledTile));

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

    // Start level creation
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
            gameState == GameState::GameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
            closeGame();

    // If on title screen and enter is pressed, start game
    if (gameState == GameState::Title && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
    {
        audio.getMusic("title").stop();
        gameState = GameState::RoundStart;
    }
}

// Sprite updater, calls each sprites update method
// with current frame and increments frame counter
void Game::update()
{
    switch (gameState)
    {
    case(GameState::Playing):
        gameTick++;                                             // *** Timing *** //

        // Decrement game seconds on second mark
        if (gameTick % _fps == 0)
            s_gameSeconds--;

        // If bomber just died and is dead                      // *** State changes *** //
        if (bomber.isDead() && bomber.hasJustDied())
        {
            if (bomber.getLives() > 0)              // If still has more lives, reset level
            {
                gameState = GameState::Death;
                levelTransition = false;
            }

            else                                    // Else, end game
            {
                gameState = GameState::GameOver;
                levelTransition = false;
            }
        }

        // If bomber is on exit and no enemies, or bonus stage ended, next level
        if ((bomber.isOnExit() && enemies.size() == 0)
            || (gameTick >= _bonusTimer && bonus))
            gameState = GameState::Transition;
        else if((bomber.isOnExit() && !enemiesKilled && !bonusPoints.has_value())) //Spawn B Panel
			bonusPoints.emplace(animations.getMisc(), getFree(), BonusPoint::Bonus::BPanel);

        // Spawn enemies for bonus stage                        // *** Enemy spawning *** //
        if (bonus)
        {
            if (stage < 40)                                             // If under 40 increment enemy type
                spawnEnemies(static_cast<Enemy::Type>((stage - 5) / 5));
            else                                                        // If 40 or above spawn Pontans
                spawnEnemies();
        }

        // Spawn Pontans if timer expired
        if (gameTick >= _pontanTimer && !levelTimerExpired)
        {
            levelTimerExpired = true;
            spawnEnemies();
        }

        bomber.update();                                        // *** Entity updates *** //

        // Enemies
        for (size_t i = 0; i < enemies.size(); i++)
        {
            enemies[i].update();

            // Kill bomber if intersecting and not a bonus stage
            if (enemies[i].getState() == Entity::State::Living &&   // Might make it to easy
                enemies[i].isOnSameTile(bomber) &&
                !bomber.hasInvinciblity())
                bomber.die();

            streak -= 1;
            if (enemies[i].getState() == Entity::State::Dead)
            {
                if(!enemiesKilled)
					enemiesKilled = true;

                //Check for combo kills
                if (streak > 0)
                    combo += 1;
                else
                    combo = 1;

                enemyType = static_cast<int>(enemies[i].getType());

                switch (enemyType)      // Update score when enemy dies
                {
                case 0: case 1: point = (enemyType + 1) * 100 * combo; break;
                case 2: case 3: point = (enemyType - 1) * 200 * combo; break;
                case 4: case 5: point = (enemyType - 3) * 1000 * combo; break;
                case 6: case 7: point = (enemyType - 5) * 2000 * combo; break;
                }
                
                s_gameScore += point;
                streak = 20;            // Waits 20 frames to check for other deaths

                points.push_back(Points(animations.getEntities(), pods, point,
                    static_cast<int>(enemies[i].getSprite().getPosition().x),
                    static_cast<int>(enemies[i].getSprite().getPosition().y)));

                enemies.erase(enemies.begin() + i);
                i--;
            }
        }

        // Bombs
        for (size_t i = 0; i < bombs.size(); i++)
        {
            bombs[i].update();

			// Remove bomb if animation is finished
            if (bombs[i].getState() == Entity::State::Dead)
            {
                bombs.erase(bombs.begin() + i);
                i--;
            }
        }

        // Explosions
        for (size_t i = 0; i < explosions.size(); i++)
        {
            explosions[i].update();

            // If bomber doesn't have fire shield and is colliding with explosion, die
            if (!(bomber.hasFireShield() || bomber.hasInvinciblity())
                && explosions[i].isOnSameTile(bomber))
                    bomber.die();

            // If explosion is colliding with enemy, kill enemy
            for (Enemy& enemy : enemies)
                if (explosions[i].isOnSameTile(enemy))
                    enemy.die();

            // If explosion is colliding with bomb, explode bomb after 3 frames
            for (Bomb& bomb : bombs)
                if (explosions[i].intersects(bomb) && !bomb.getWillExplode())
                    bomb.delay();       // Explodes in 5 frames

            // If explosion is colliding with powerup, spawn enemies and remove powerup
            if (powerUp && powerUp->intersects(explosions[i]))
            {
                spawnEnemies(getEnemyType());     // Spawn enemies if powerup is hit by explosion
                powerUp.reset();                  // Remove powerup if hit by explosion
            }

            // If explosion tile is the exit, spawn enemies
            if (pods[explosions[i].getY()][explosions[i].getX()].isExit)
                spawnEnemies(static_cast<Enemy::Type>(powerupPresets[stage]));

			// Remove explosion if animation is finished
            if (explosions[i].getState() == Entity::State::Dead)
            {
                explosions.erase(explosions.begin() + i);
                i--;
            }
        }

        // Softwalls
        for (size_t i = 0; i < softWalls.size(); i++)
        {
            Pod& pod = pods[softWalls[i].getY()][softWalls[i].getX()];

            if (pod.isDying)
            {
                pod.isDying = false;
                pod.isSoft = false;
                pod.isFilled = false;

                if (softWalls[i].isOnSameTile(bomber))
                    bomber.die();
            }

            softWalls[i].update();

			// Remove soft wall if animation is finished
            if (softWalls[i].getState() == Entity::State::Dead)
            {
                softWalls.erase(softWalls.begin() + i);
                i--;
            }
        }

        // Points
        for (size_t i = 0; i < points.size(); i++)
        {
            points[i].update();

            if (points[i].getState() == Entity::State::Dead)
            {
                points.erase(points.begin() + i);
                i--;
            }
        }

        //BonusPoints
        if(bonusPoints && bonusPoints->intersects(bomber))
        {
            bonusPoints->applyEffect(s_gameScore);
            panel.updatePowerUp(bonusPoints->getType());
            cout << *bonusPoints << "\n";
            bonusPoints.reset();
        }

        // Powerup
        if (powerUp && powerUp->intersects(bomber))
        {
            powerUp->applyEffect(bomber);
            panel.updatePowerUp(powerUp->getType());
            cout << *powerUp << "\n";
            powerUp.reset();
        }

        // Invincibility display                                // *** UI updates *** //
        if (bomber.hasInvinciblity())                           // If bomber is invincible
        {
            if (invincibilePlayerTicks >= _invincibilityTimer)      // If the timer runs out, remove powerup and reset icon
            {
                active = false;
                invincibilePlayerTicks = 0;
                bomber.removeInvincibility();
                panel.updatePowerUp(PowerUp::Type::Invincible, active);
            }

            int interval = 0;                                       // Default interval tick

            if (invincibilePlayerTicks >= _fastBlinkSpeed)          // Set blink timing based on how much time is left
                interval = _fastBlinkInterval;
            else if (invincibilePlayerTicks >= _mediumBlinkSpeed)
                interval = _mediumBlinkInterval;
            else if (invincibilePlayerTicks >= _slowBlinkSpeed)
                interval = _slowBlinkInterval;

            if (interval > 0 && invincibilePlayerTicks % interval == 0)     // If on interval timing
            {
                panel.updatePowerUp(PowerUp::Type::Invincible, active);         // Blink the powerup display
                active = !active;                                               // Swap the sign to opposite
            }

            invincibilePlayerTicks++;
        }

        panel.update();

        break;

    case(GameState::RoundStart):
		// Stop music and play round start audio, display current level,
        // then wait until audio finishes to start level and music
        audio.getMusic(song).stop();

        if (!levelTransition)
        {
            audio.playSound("roundStart");

            if (bonus)
                textObjects.emplace_back("bonus stage", _centerScreen);
            else
                textObjects.emplace_back("stage " + std::to_string(stage + 1), _centerScreen);

            levelTransition = true;
        }

        // Wait until audio finishes
        if (audio.getStatus("roundStart") == sf::SoundSource::Status::Stopped)
        {
            textObjects.clear();

            gameState = GameState::Playing;

            if (bonus)
            {
                audio.getMusic("bonusStage").play();
                audio.getMusic("bonusStage").setLooping(true);
                song = "bonusStage";

                bomber.invincible();
                panel.updatePowerUp(PowerUp::Type::Invincible);
            }

            else
            {
                audio.getMusic("main").play();
                audio.getMusic("main").setLooping(true);
                song = "main";
            }

            levelTransition = false;
        }

        break;

    case(GameState::Transition):
		// Stop music and play stage clear audio, display stage clear,
        // then wait until audio finishes to start next level and music
        audio.getMusic(song).stop();

        if (!levelTransition)
        {
            audio.playSound("stageClear");
            textObjects.emplace_back("stage clear", _centerScreen);
            levelTransition = true;

            if (!bonus)
                stage++;
            if (bomber.getLives() < 3)
            {
                bomber.addLife();
                panel.updateLives(true);
            }
            if (stage != 1 && stage % 5 == 0 && !bonus)
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
            textObjects.clear();

            if (!bonus)
                level();

            gameState = GameState::RoundStart;
            levelTransition = false;
        }

        break;

	case (GameState::Death):
        // Stop music and play miss audio, display "you died",
        // then wait until audio finishes to either restart level or show game over screen
        audio.getMusic(song).stop();

        if (!levelTransition)
        {
            audio.playSound("miss");
            levelTransition = true;
            panel.updateLives();
        }

        // Wait until audio finishes
        if (audio.getStatus("miss") == sf::SoundSource::Status::Stopped)
        {
            level();

            gameState = GameState::RoundStart;
            levelTransition = false;
        }
		break;

    case (GameState::GameOver):
		// Stop music and play game over audio, display game over,
		// then wait until audio finishes to show game over screen
        audio.getMusic(song).stop();

        if (!levelTransition)
        {
            audio.playSound("gameOver");
            textObjects.emplace_back("game over", _centerScreen);
            levelTransition = true;
        }

        // Wait until audio finishes
        if (audio.getStatus("gameOver") == sf::SoundSource::Status::Stopped)
        {
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

    switch (gameState)
    {
    case(GameState::Playing):
    case(GameState::Death):
        window.setView(world);

        window.draw(background);

        if (powerUp)
            window.draw(powerUp->getSprite());

        if(bonusPoints)
			window.draw(bonusPoints->getSprite());

        for (SoftWall& wall : softWalls)
            window.draw(wall);

        for (Bomb& bomb : bombs)
            window.draw(bomb);

        for (Explosion& explosion : explosions)
            window.draw(explosion);

        window.draw(bomber);

        for (Enemy& enemy : enemies)
            window.draw(enemy);

        for (Points& point : points)
            window.draw(point);

        window.setView(UI);

        window.draw(panel);

        break;

    case(GameState::GameOver):
		if (gameOver)                       // If game over audio has finished, show game over screen
        {
            window.draw(endTitle);
            break;
        }
        [[fallthrough]];
	case(GameState::RoundStart):            // Display text during music transitions
    case(GameState::Transition):            // for round start, stage clear, and game over
        for (Text& text : textObjects)
            for (sf::Sprite& glyph : text.sprites)
                window.draw(glyph);

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

// Called at the start of each level,
// clears the map and then fills it
// with soft walls, an exit, a powerup,
// and enemies based on the current stage
void Game::level()
{
    clear();

    int walls = stage * 2 + 54;
    int playable = _softPods;
	bool enemiesKilled = false;

    for (int row = 0; row < _rows; row++)
    {
        if (walls == 0)         // Need to break out of both loops if no more walls
            break;

        for (int col = 0; col < _cols; col++)
        {
            if (walls == 0)         // Break if no more walls
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
                    softWalls.emplace_back(animations.getEntities(), pods, col, row);
                    walls--;
                }

                playable--;
            }
        }
    }

	if (softWalls.size() == 0)          // If no soft walls, can't place exit and powerup, so just return to prevent errors
    {
        cout << "No soft walls to place exit and powerup on\n";
        return;
    }

    int exitIndex = rand() % softWalls.size();          // Get random index for exit and powerup in soft wall vector
    int powerUpIndex = rand() % softWalls.size();

    while (powerUpIndex == exitIndex)                       // If they are the same, get a new random
        powerUpIndex = rand() % softWalls.size();           // index for powerup until they are different

    int x = softWalls[exitIndex].getX();                // Get x and y of selected exit wall
    int y = softWalls[exitIndex].getY();
    pods[y][x].isExit = true;                               // Set selected pod to be exit
    cout << "Exit set at: (" << x << ", " << y << "), ";

	x = softWalls[powerUpIndex].getX();                 // Get x and y of selected powerup wall
	y = softWalls[powerUpIndex].getY();
    powerUp.emplace(PowerUp(animations.getMisc(),           // Make a new powerup at the selected powerup position
        static_cast<PowerUp::Type>(powerupPresets[stage]), x, y));
    cout << "Powerup set at: (" << x << ", " << y << ")\n";

    for (int enemyType = 0; enemyType < 8; enemyType++)                     // Runs the enemy create loop per enemy type,
        for (int i = 0; i < enemyPresets[stage % 50][enemyType]; i++)           // for how many of each enemy type to spawn
        {                                                                       // based on the presets for the current stage
            Enemy enemy(animations.getEntities(), pods, static_cast<Enemy::Type>(enemyType), bomber);
            int x, y;

            do
            {
                x = rand() % (_cols - 3) + 3;
                y = rand() % (_rows - 3) + 3;
            } while (pods[y][x].isFilled);

            enemy.setPosition(x, y);
            enemies.emplace_back(enemy);
        }
}

// Called before each level to reset the
// map, and get rid of last level's data
void Game::clear()
{
    for (int row = 0; row < _rows; row++)   // Reset all pods not walls or borders to empty
        for (int col = 0; col < _cols; col++)
        {
            bool isInnerWall = col % 2 == 0 && row % 2 == 0;
            bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;

            if (!(isInnerWall || isBorder))
                pods[row][col].isBomb = pods[row][col].isExit = pods[row][col].isFilled = pods[row][col].isSoft = false;
        }

	bomber.reset();                         // Reset bomber position and status
	enemies.clear();                        // Clear all containers of various items
    bombs.clear();
    softWalls.clear();
    explosions.clear();
    points.clear();
	powerUp.reset();

    if(bonus)                               // Reset misc values for new level
        s_gameSeconds = _bonusTimer / _fps;
    else
        s_gameSeconds = _pontanTimer / _fps;

	gameTick = 0;
    invincibilePlayerTicks = 0;
    levelTimerExpired = false;
    active = false;

    bomber.removeInvincibility();           // Make sure invincibility is gone after bonus stage
    panel.updatePowerUp(PowerUp::Type::Invincible, active);
}

// Called after a powerup or exit is hit, the pontan
// timer expires, or on bonus stage, spawns enemies of
// given type and places them randomly on the map
void Game::spawnEnemies(Enemy::Type type)
{
    int size = enemies.size();          // Number of current enemies
    int copy = std::min(size, 10);      // Number of enemies needed to copy back to vector
    int spawn = 10 - copy;              // Number of new enemies needed to spawn

    if (levelTimerExpired)              // If timer expired for Pontans
    {
        std::vector<Enemy> newEnemies;
        newEnemies.reserve(10);

        for (int i = 0; i < copy; i++)      // For how ever many enemies are currently
        {                                   // alive, replace them with a new Pontan
            Enemy enemy(animations.getEntities(), pods, type, bomber);
            enemy.setPosition(enemies[i].getX(), enemies[i].getY());

            newEnemies.emplace_back(enemy);     // Place new enemy in Pontan vector
        }

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
}

// Gets the enemy type to spawn based on
// the current powerup, used for spawning
// enemies when a powerup is hit by an explosion
Enemy::Type Game::getEnemyType() const
{
    if (!powerUp)
    {
        cout << "No powerup to get enemy type from\n";
        return Enemy::Type::Ballom;     // Default enemy type if no powerup, should never be used
    }

    switch (powerUp->getType())
    {
    case PowerUp::Type::ExtraBomb:  return Enemy::Type::Ballom;
    case PowerUp::Type::ExtraRange: return Enemy::Type::Onil;
    case PowerUp::Type::Skate:      return Enemy::Type::Dahl;
    case PowerUp::Type::WallPhase:  return Enemy::Type::Minvo;
    case PowerUp::Type::Remote:     return Enemy::Type::Doria;
    case PowerUp::Type::BombPhase:  return Enemy::Type::Ovape;
    case PowerUp::Type::FireShield: return Enemy::Type::Pass;
    case PowerUp::Type::Invincible: return Enemy::Type::Pontan;
    }
}

std::pair<int, int> Game::getFree() //Find free position for bonus points
{
    int x, y;
    do
    {
        x = rand() % (_cols - 1) + 1;
        y = rand() % (_rows - 1) + 1;
    } while (pods[y][x].isFilled);
    return { x  , y };
}