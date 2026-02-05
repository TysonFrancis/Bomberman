#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Pod.h"


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
	std::vector<Pod> pods;
	std::vector<Pod> walls;

	int width = 13;
	int depth = 11;
	int x = width / 2 + 1;
	int y = depth / 2;
	int num1 = (x + width) * y + width;
	int num2 = (x - 1) * (y);
};