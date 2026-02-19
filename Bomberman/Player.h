#pragma once
#include <SFML/Graphics.hpp>
#include"Pod.h"
#include "Entity.h"

/*
	Player class that extends entity not owning its own sprite,
	but has access to it. Addpends new member variables as needed.
	Update method takes in the current frame of animation to update the sprite.
*/

class Player : public Entity
{
public:
	Player(sf::Texture&, Pod[11][29]);

	void update();

private:
	Pod pods[11][29];
	float joyX, joyY;
	float speed;
	int x, y;
};