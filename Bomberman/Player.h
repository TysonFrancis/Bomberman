#pragma once
#include <SFML/Graphics.hpp>

#include"Pod.h"
#include "Entity.h"
#include "Constants.h"

using namespace Constants;

/*
	Player class that extends entity not owning its own sprite,
	but has access to it. Addpends new member variables as needed.
	Update method takes in the current frame of animation to update the sprite.
*/

class Player : public Entity
{
public:
	Player(const sf::Texture&, Pod (&pod)[_rows][_cols]);

	void update();

	// For testing purposes, prints the player's position
	friend std::ostream& operator<<(std::ostream&, const Player&);

private:
	Pod (&pods)[_rows][_cols];
	float joyX, joyY;
	float speed;
	int x, y;
};