#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Pod.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Constants.h"

/*
	Player class that extends entity and its protected
	member variables, with added player specific ones.
	Representing the bomber and its movement,
	collision, and other neccessary items.

	Constructor takes in a texture and 2D pod array to
	pass to entity, as well as Bomb and Explosion vectors.
*/

class Player : public Entity
{
public:
	Player(const sf::Texture&, Pod (&pods)[Constants::_rows][Constants::_cols],
		std::vector<Bomb>&, std::vector<Explosion>&);

	void update();
	void animate();
	void die();

	void extraBomb();
	void extraRange();
	void giveRemote();
	void giveSkate();
	void phaseWalls();
	void phaseBombs();
	void shieldFire();
	void invincible();

	bool hasFireShield() const { return isFireShield; }
	bool hasInvincible() const { return isInvincible; }

	friend std::ostream& operator<<(std::ostream&, const Player&);
	Player& operator=(const Player&);

private:
	void moveLogic();
	bool isObstructed(int, int);

	std::vector<Bomb>& bombs;
	std::vector<Explosion>& explosions;

	float speed;

	int joyX, joyY;
	int lives;

	int blast;		// Used to determine blast radius of bombs
	int maxBombs;	// Used to determine how many bombs player can have out at once
	bool remote;	// Used to determine if player has remote control powerup, default false
	int wait = 0;	// Delay between exploding bombs

	bool isFireShield;
	bool isInvincible;
};