#include <SFML/Graphics.hpp>
#include "Player.h"
#pragma once

class Game
{
public:
	Game();
	~Game();

	void run();
	void tick();

	Player player;

private:
	sf::RenderWindow window;
	sf::CircleShape tempShape;
};