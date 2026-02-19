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

	// ********** TYSON LOGIC STUFF ********** START ********** //
	sf::RectangleShape player;

	Pod pods[11][29]; //Array of Pods , 11 rows, 29 columns, 
	int playindex = 0;

	// ********** END ********** //

private:
	Animations animations;		// Animations MUST be declared before all entity
	Entity title;				// objects so textures are loaded before entity
	Entity background;			// creation. Best practice is to match initialization
	Player bomber;				// list order with class declaration order.

	unsigned int windowWidth, windowHeight;
	sf::RenderWindow window;

	GameState state;
	int frame;
};