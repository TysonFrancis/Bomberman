#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();
	~Player();

private:
	sf::Sprite sprite;
	sf::Texture texture;
};

