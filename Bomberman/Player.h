#pragma once
#include "Entity.h"
#include "Constants.h"

class Bomb;
class Explosion;
class Pod;

/*
	Player class, represents moveable character
	controlled by person, can place bombs, use
	powerups and is killed by enemies and explosions.
*/

class Player : public Entity
{
public:
	Player(const sf::Texture&, Pod (&pods)[Constants::_rows][Constants::_cols],
		std::vector<Bomb>&, std::vector<Explosion>&);

	void update();
	void animate();
	void die();

	void reset();

	void extraBomb();
	void extraRange();
	void giveRemote();
	void giveSkate();
	void phaseWalls();
	void phaseBombs();
	void shieldFire();
	void invincible();

	bool hasFireShield() const;
	bool hasInvincible() const;

	int getLives() const;
	void addLife();

	bool isOnExit() const;
	bool isDead() const;
	bool hasJustDied();

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
	int wait;		// Delay between exploding bombs
	bool remote;	// Used to determine if player has remote control powerup, default false

	bool isFireShield;
	bool isInvincible;
	bool wallPhase;
	bool bombPhase;

	bool justDied;		// Used to determine if player just died, for checking if can respawn or should fully die
};