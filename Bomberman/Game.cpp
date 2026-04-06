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
    timerExpired(false)
{
    srand((static_cast<unsigned>(time(nullptr))));

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
            //bool isSoft = (rand() % 4 == 0) && (row > 2 || col > 2); // Can't spawn in top 2 x 2 by player

            if (isInnerWall || isBorder)
            {
                pods[row][col].isFilled = true;
                pods[row][col].isHard = true;
            }
            /*
            else if (isSoft)
            {
                pods[row][col].isFilled = true;
                pods[row][col].isSoft = true;
                softWalls.push_back(SoftWall(animations.getEntities(), pods, col, row));
            }
            */
        }

	// Set exit and powerup in random soft wall positions
    /*
	int exit = rand() % softWalls.size();                                   // Get random index for exit and powerup in soft wall vector
    int powerUp = 0;

	while (powerUp == exit)                                                 // If they are the same, get a new random
        powerUp = rand() % softWalls.size();                                // index for powerup until they are different

	pods[softWalls[exit].getY()][softWalls[exit].getX()].isExit = true;     // Set selected pod to be exit
	powerUps.push_back(PowerUp(animations.getMisc(), pods,                  // Make a new powerup of random type at the selected powerup position
        static_cast<PowerUp::Type>(rand() % 8), softWalls[powerUp].getX(), softWalls[powerUp].getY()));

    cout << "Exit set at: (" << softWalls[exit].getX() << ", " << softWalls[exit].getY() << ")\n"
        << "Powerup set at: (" << softWalls[powerUp].getX() << ", " << softWalls[powerUp].getY() << ")\n";

    // Make more powerups
    /*
    for (int i = 0; i < 9; i++)
    {
        powerUp = rand() % softWalls.size();

        while (powerUp == exit)
			powerUp = rand() % softWalls.size();

        powerUps.push_back(PowerUp(animations.getMisc(), pods, static_cast<PowerUp::Type>(rand() % 8), softWalls[powerUp].getX(), softWalls[powerUp].getY()));

		cout << "Powerup set at: (" << softWalls[powerUp].getX() << ", " << softWalls[powerUp].getY() << ")\n";
    }
    */
    // Make 5 enemies and put them in positions that are empty
    /*
    for (int i = 0; i < 10; i++)
    {
        Enemy enemy(animations.getEntities(), pods, static_cast<Enemy::Type>(rand() % 6), bomber);
        int x, y;

        do
        {
            x = rand() % (_cols - 3) + 3;
            y = rand() % (_rows - 3) + 3;
        } while (pods[y][x].isFilled);

        enemy.setPosition(x, y);
        enemies.push_back(enemy);
        s_enemyCount++;
    }
    */
}

// Holds main game loop, all actions passed
// to supplementary methods for cleanliness
void Game::run()
{
    int enemy[8][50] = {};
    int powerups[50] = {};
    int levels = 0;
    s_gameState = GameState::Title;
    while (window.isOpen())
    {
        level(levels, enemy, powerups[levels]);
        s_gameState = GameState::RoundStart;
        
        levels++;
        while(bomber.getLives() <= 0)
            events(0);
    }
    
}

// Handles window events like game starting and closing
void Game::events(int stage)
{
    // Close game if window is closed, escape key pressed, or on game over screen and enter pressed
    while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape) ||
            s_gameState == GameState::GameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter))
            closeGame();

    // If on title screen and enter is pressed, start game
    if (s_gameState == GameState::Title && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Enter)||s_gameState==GameState::RoundStart)
    {
        s_gameState = GameState::RoundStart;
        cout << "round start\n";

        audio.getRoundStart().play(); // Play silly music
        if(stage>0)
            textObjects.push_back(new Text("Stage "+stage, {0, 0}));
        else
            textObjects.push_back(new Text("Bonus Stage", { 0, 0 }));
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

        //if (gameTick % _fps == 0)                           // Display game seconds
        //    cout << "seconds: " << gameTick / _fps << endl;
        if (gameTick / _fps >= 200 && !timerExpired)        // Spawn Pontans if past x seconds
            spawnPontans();

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

        for (size_t i = 0; i < softWalls.size(); i++)
        {
            softWalls[i].update();

            if (softWalls[i].getState() == Entity::State::Dead)
            {
                softWalls.erase(softWalls.begin() + i);
                i--;
            }
        }

        for (size_t i = 0; i < powerUps.size(); i++)
        {
            if (powerUps[i].intersects(bomber))
            {
				powerUps[i].applyEffect(bomber);
                cout << powerUps[i];
                powerUps.erase(powerUps.begin() + i);
                i--;
            }
		}

        break;

    case(GameState::RoundStart):

        while (audio.getRoundStart().getStatus() == sf::SoundSource::Status::Playing)
        {
            render();
        }
        // Wait for music to finish
        if (audio.getRoundStart().getStatus() == sf::SoundSource::Status::Stopped)
        {
            // Might cause issues later if other text gets added to the screen before the stage number
            // A for loop fixes this no?
            for (size_t i = 0; i < textObjects.size(); i++)
            {
                delete textObjects.back();
                textObjects.pop_back();
            }

            cout << "playing game\n";
            s_gameState = GameState::Playing;
        }
        
        cout << "almost playing game\n";

        break;

    case(GameState::GameOver):  gameTick++;     break;
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

        for (PowerUp& powerUp : powerUps)
            window.draw(powerUp);

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

	case(GameState::RoundStart):
        for (Text* text : textObjects)
            for (sf::Sprite* glyph : text->sprites)
                window.draw(*glyph);

        break;

    case(GameState::Title):       window.draw(title);          break;
	case(GameState::GameOver):    window.draw(endTitle);       break;
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


// Called after 200 game seconds have passed on one single level,
// removes all enemies from the board and replaces them with Pontans,
// adding or subtracting as needed until there are 10
void Game::spawnPontans()
{
    timerExpired = true;

    int size = enemies.size();          // Number of current enemies
    int copy = std::min(size, 10);      // Number of enemies needed to copy back to vector
    int spawn = 10 - copy;              // Number of new enemies needed to spawn

    std::vector<Enemy> newEnemies;
    newEnemies.reserve(10);

    for (int i = 0; i < copy; i++)      // For how ever many enemies are currently
    {                                   // alive, replace them with a new Pontan
        Enemy enemy(animations.getEntities(), pods, Enemy::Type::Pontan, bomber);
        enemy.setPosition(enemies[i].getX(), enemies[i].getY());

        newEnemies.push_back(enemy);        // Place new enemy in Pontan vector
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
        newEnemies.push_back(enemy);        // Place new enemy in Pontan vector
    }

    enemies = std::move(newEnemies);    // Copy over all Pontans back into original enemies vector
    s_enemyCount = enemies.size();      // Re-initialize enemy count
}


void Game::level(int level, int enemyCount[8][50], int power)
{
    cout << "level: " << level << "\n";
    bomber.setPosition(1, 1);
    //insert pod clearing method
    //clear other vectors 
    int walls = level * 2 + 54;
    int playable = _softPods;
    for (int row = 0; row < _rows; row++)
    {
        for (int col = 0; col < _cols; col++)
        {
            if (walls == 0)
                break;
            bool isSoft = (rand() % (playable/walls) == 0) && (row > 2 || col > 2); // Can't spawn in top 2 x 2 by player
            bool isInnerWall = col % 2 == 0 && row % 2 == 0;
            bool isBorder = col == 0 || col == _cols - 1 || row == 0 || row == _rows - 1;
            if(!(isInnerWall||isBorder))
            {
                if (isSoft)
                {
                    pods[row][col].isFilled = true;
                    pods[row][col].isSoft = true;
                    softWalls.push_back(SoftWall(animations.getEntities(), pods, col, row));
                    walls--;
                }
                playable--;
            }
            
        }
    }

    int exit = rand() % softWalls.size();                                   // Get random index for exit and powerup in soft wall vector
    int powerUp = 0;

    while (powerUp == exit)                                                 // If they are the same, get a new random
        powerUp = rand() % softWalls.size();                                // index for powerup until they are different

    pods[softWalls[exit].getY()][softWalls[exit].getX()].isExit = true;     // Set selected pod to be exit
    powerUps.push_back(PowerUp(animations.getMisc(), pods,                  // Make a new powerup of random type at the selected powerup position
        static_cast<PowerUp::Type>(power), softWalls[powerUp].getX(), softWalls[powerUp].getY()));

    cout << "Exit set at: (" << softWalls[exit].getX() << ", " << softWalls[exit].getY() << ")\n"
        << "Powerup set at: (" << softWalls[powerUp].getX() << ", " << softWalls[powerUp].getY() << ")\n";
    for(int k=0;k<8;k++)
    {
        for (int i = 0; i < enemyCount[k][level]; i++)
        {
            Enemy enemy(animations.getEntities(), pods, Enemy::Type(k), bomber);
            int x, y;

            do
            {
                x = rand() % (_cols - 3) + 3;
                y = rand() % (_rows - 3) + 3;
            } while (pods[y][x].isFilled);

            enemy.setPosition(x, y);
            enemies.push_back(enemy);
            s_enemyCount++;
        }
    }

    while(s_gameState!=GameState::GameOver && s_gameState != GameState::Transition)
    {
        events(level + 1);
        update();
        render();
    }
}