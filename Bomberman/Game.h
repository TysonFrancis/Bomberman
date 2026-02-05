#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Animations.h"
#include "Windows.h"
#pragma once

class Game
{
public:
	Game();

	void run();

private:
	sf::RenderWindow window;
	Player bomber;

	bool alive;
	int count;
};