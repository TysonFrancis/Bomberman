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
#include "HardWall.h"
#include "SoftWall.h"
#include "Constants.h"

/*
	Main class handling game logic and display,
	has all necessary items for sfml viewing and
	different sprites. Methods to run the game loop,
	handle events, update sprites, and render the sprites.
	Destructs game at window close and/or game win or lose.
	GameState enum to determine what to update and render.
*/

enum class GameState
{
	Title,
	RoundStart,
	Playing,
	GameOver // Future
};

class Game
{
public:
	Game();

	void run();
	void events();
	void update();
	void render();
	void startRound();
	void closeGame();

private:
	Animations animations;
	Audio audio;
	Entity title;
	Entity background;
	Player bomber;
	std::vector<Enemy> enemies;

	Pod pods[Constants::_rows][Constants::_cols]; // Array of Pods, 13 rows, 31 columns
	std::vector<Bomb> bombs;
	int bombCount;
	sf::RenderWindow window;

	GameState state;
	int frame;
	
};