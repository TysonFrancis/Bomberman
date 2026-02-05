#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#pragma once

class Game
{
public:
	Game();

	void run();
	void tick();

	Player player;

private:
	sf::RenderWindow window;
	sf::Clock clock;
};