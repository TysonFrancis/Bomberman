#pragma once
#include <SFML/Graphics.hpp>

#include "Pod.h"
#include "Entity.h"
#include "Constants.h"
#include "Bomb.h"

/*
	Player class that extends entity not owning its own sprite,
	but has access to it. Appends new member variables as needed.
	Update method takes in the current frame of animation to update the sprite.
*/

class Player : public Entity
{
public:
	Player(const sf::Texture&, Pod (&pod)[Constants::_rows][Constants::_cols], std::vector<Bomb>& bombs);

	void update();
	void animate();

	// For testing purposes, prints the player's position
	friend std::ostream& operator<<(std::ostream&, const Player&);

private:
	Pod (&pods)[Constants::_rows][Constants::_cols];
	float speed, joyX, joyY;
	int x, y;
	std::vector<Bomb>& bombs;

	bool remote = false; // Used to determine if player has remote control powerup, default false
	int blast = 1; // Used to determine blast radius of bombs
	int maxBombs = 1; // Used to determine how many bombs player can have out at once
};