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

/*
	Main class handling game logic and display,
	has all necessary items for sfml viewing and
	different sprites. Methods to run the game loop,
	handle events, update sprites, and render the sprites.
	Destructs game at window close and/or game win or lose.
	GameState enum to determine what to update and render.
*/

enum class GameState { Title, RoundStart, Playing, GameOver };

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
	void startRound();
	void closeGame();
	void spawnPontans();

	Animations animations;

	Player bomber;
	std::vector<Enemy> enemies;

	std::vector<Bomb> bombs;
	std::vector<Explosion> explosions;
	std::vector<SoftWall> softWalls;

	Pod pods[Constants::_rows][Constants::_cols];

	Audio audio;
	sf::Sprite title;
	sf::Sprite endTitle;
	sf::Sprite background;

	sf::RenderWindow window;

	int gameTick;
	int score;
	int streak; //Checks if the player has killed enemies in quick sucession
	int combo;
	int enemyType;

	bool isExit;
	bool timerExpired;
};