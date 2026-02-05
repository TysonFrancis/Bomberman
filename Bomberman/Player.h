#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player : public Entity
{
public:
	Player(sf::Texture&);
	~Player();

	void tick(sf::RenderWindow&);

private:
	sf::Sprite sprite;
	float joyX, joyY;
	sf::RectangleShape tempDisplay;
};