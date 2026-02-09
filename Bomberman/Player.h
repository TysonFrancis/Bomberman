#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"

/*
	Player class that extends entity not owning its own sprite,
	but has access to it. Addpends new member variables as needed.
	Update method takes in the current frame of animation to update the sprite.
*/

class Player : public Entity
{
public:
	Player(sf::Texture&);

	void update();

	sf::FloatRect getBounds() const;

private:
	float joyX, joyY;
	float speed;
};