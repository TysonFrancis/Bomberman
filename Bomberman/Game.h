#include <SFML/Graphics.hpp>
#pragma once

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	sf::RenderWindow window;
};