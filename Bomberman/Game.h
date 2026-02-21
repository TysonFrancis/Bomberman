#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Animations.h"
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
	Playing
};

class Game
{
public:
	Game();

	void run();
	void events();
	void update();
	void render();
	void closeGame();

	// Old Tyson stuff
	sf::RectangleShape player;
	int playindex = 0;
	//

private:
	Animations animations;
	Entity title;
	Entity background;
	Player bomber;

	Pod pods[_rows][_cols]; // Array of Pods, 13 rows, 31 columns

	sf::RenderWindow window;

	GameState state;
	int frame;
};