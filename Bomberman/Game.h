#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Animations.h"
#include "Entity.h"
#include "Player.h"
#include "Bomb.h"
#include "Enemy.h"
#include "Pod.h"

/*
	Main class handling game logic and display,
	has all necessary items for sfml viewing and
	different sprites. Methods to run the game loop,
	handle events, update sprites, and render the sprites.
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

	// ********** TYSON LOGIC STUFF ********** START ********** //
	sf::CircleShape player;

	std::vector<Pod> pods;
	std::vector<Pod> walls;
	int playindex = 0;

	//setting game size numbers
	int width = 11;
	int depth = 9;
	int x = width / 2 + 1;
	int y = depth / 2;
	int num1 = (x + width) * y + width;
	int num2 = (x - 1) * (y);
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