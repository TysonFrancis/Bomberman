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
	Player(const sf::Texture&, Pod (&pods)[Constants::_rows][Constants::_cols], std::vector<Bomb>& bombs);

	void update();
	void animate();
	void die();

	friend std::ostream& operator<<(std::ostream&, const Player&);
	Player& operator=(const Player& other);

private:
	void moveLogic();
	bool isObstructed(int, int);

	std::vector<Bomb>& bombs;

	float speed;

	int joyX, joyY;
	int lives;

	int blast;		// Used to determine blast radius of bombs
	int maxBombs;	// Used to determine how many bombs player can have out at once
	bool remote;	// Used to determine if player has remote control powerup, default false
};