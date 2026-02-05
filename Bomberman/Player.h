#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();
	~Player();

	void tick();
	sf::CircleShape shape;
private:
	//sf::Sprite sprite;
	//sf::Texture texture;
	float joyX, joyY;
};