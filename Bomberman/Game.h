#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Animations.h"
#include "Audio.h"
#include "Entity.h"
#include "Player.h"
#include "Bomb.h"
#include "Enemy.h"
#include "Pod.h"
#include "SoftWall.h"
#include "Explosion.h"
#include "Constants.h"
#include "Text.h"
#include "PowerUp.h"

/*
	Main class handling game logic and display,
	has all necessary items for sfml viewing and
	different sprites. Methods to run the game loop,
	handle events, update sprites, and render the sprites.
	Destructs game at window close and/or game win or lose.
	GameState enum to determine what to update and render.
*/

enum class GameState { Title, RoundStart, Transition, Playing, GameOver };

class Game
{
public:
	Game();

	void run();

	// For player to be able to access and change
	// game state if say player dies or finds exit,
	// also prefixed with s_ to know its static
	inline static GameState s_gameState = GameState::Title;

	// For player to be able to know when enemy count
	// reaches 0 without passing vector as parameter,
	// for checking if can end the level
	inline static int s_enemyCount = 0;

private:
	void events();
	void update();
	void render();
	void closeGame();

	void spawnEnemies(Enemy::Type);
	void level();
	void clear();

	Enemy::Type getEnemyType() const;

	Animations animations;

	Player bomber;
	std::vector<Enemy> enemies;

	std::vector<Bomb> bombs;
	std::vector<Explosion> explosions;
	std::vector<SoftWall> softWalls;
	std::optional<PowerUp> powerUp;

	std::vector<Text*> textObjects;

	Pod pods[Constants::_rows][Constants::_cols];

	Audio audio;

	sf::Sprite title;
	sf::Sprite endTitle;
	sf::Sprite background;

	sf::RenderWindow window;

	// Each number or vector represents the type of powerup
	// or enemy to spawn for each level, for 50 levels
	int powerupPresets[50] =
	{
		1,0,4,2,0,
		0,1,4,5,3,
		0,0,4,5,1,
		3,0,5,0,4,
		5,4,0,4,3,
		7,1,0,4,6,
		3,0,4,7,5,
		6,4,1,3,7,
		4,3,5,4,7,
		3,5,4,6,7
	};
	int enemyPresets[50][8] =
	{
		{6,0,0,0,0,0,0,0}, {3,3,0,0,0,0,0,0}, {2,2,2,0,0,0,0,0}, {1,1,2,2,0,0,0,0}, {0,4,3,0,0,0,0,0},
		{0,2,3,2,0,0,0,0}, {0,2,3,0,2,0,0,0}, {0,1,2,4,0,0,0,0}, {0,1,1,4,0,1,0,0}, {0,1,1,1,1,3,0,0},
		{0,1,2,3,1,1,0,0}, {0,1,1,1,1,4,0,0}, {0,0,3,3,0,2,0,0}, {0,0,0,0,7,0,1,0}, {0,0,1,3,0,3,1,0},
		{0,0,0,3,0,4,1,0}, {0,0,5,0,0,2,1,0}, {3,3,0,0,0,0,2,0}, {1,1,3,0,1,0,2,0}, {0,1,1,1,1,2,2,0},
		{0,0,0,0,3,4,2,0}, {0,0,4,3,0,1,1,0}, {0,0,2,2,2,2,1,0}, {0,0,1,1,2,4,1,0}, {0,2,1,1,2,2,1,0},
		{1,1,1,1,2,2,1,0}, {1,1,0,0,1,5,1,0}, {0,1,3,3,0,1,1,0}, {0,0,0,0,5,2,2,0}, {0,0,3,2,2,1,1,0},
		{0,2,2,2,2,2,0,0}, {0,1,1,3,0,4,0,0}, {0,0,2,2,1,3,1,0}, {0,0,2,3,0,3,1,0}, {0,0,2,1,1,3,2,0},
		{0,0,2,2,0,3,2,0}, {0,0,2,1,1,3,2,0}, {0,0,2,2,0,3,3,0}, {0,0,1,1,2,2,3,0}, {0,0,1,2,0,3,3,0},
		{0,0,1,1,1,3,3,0}, {0,0,0,1,1,3,5,0}, {0,0,0,1,1,2,5,0}, {0,0,0,1,1,2,6,0}, {0,0,0,0,2,2,5,0},
		{0,0,0,0,2,2,5,0}, {0,0,0,0,2,2,5,0}, {0,0,0,0,1,2,6,1}, {0,0,0,0,2,1,6,1}, {0,0,0,0,2,1,5,2}
	};

	int gameTick;
	int score;
	int streak; //Checks if the player has killed enemies in quick sucession
	int combo;
	int enemyType;

	int stage;
	bool levelTransition;
	bool timerExpired;
	bool gameOver;
	bool bonus;

	const char* song;		// To know which song to stop when level change or game over
};