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
	void gameReset();

	void extraBomb();
	void extraRange();
	void giveSkate();
	void phaseWalls();
	void giveRemote();
	void phaseBombs();
	void shieldFire();
	void invincible();

	int  getMaxBombs() const;
	int  getBlast() const;
	bool hasSkate() const;
	bool hasWallPhase() const;
	bool hasRemote() const;
	bool hasBombPhase() const;
	bool hasFireShield() const;
	bool hasInvinciblity() const;

	void removeInvincibility();

	int getLives() const;
	void addLife();

	bool isOnExit() const;
	bool isDead() const;
	bool hasJustDied();

	friend std::ostream& operator<<(std::ostream&, const Player&);
	Player& operator=(const Player&);

	std::pair<int, int> getJoy() const { return { joyX, joyY }; }//For Cola

private:
	void moveLogic();
	bool isObstructed(int, int);

	std::vector<Bomb>& bombs;
	std::vector<Explosion>& explosions;

	float speed;

	int joyX, joyY;
	int lives;

	int maxBombs;	// Used to determine how many bombs player can have out at once
	int blast;		// Used to determine blast radius of bombs
	int wait;		// Delay between exploding bombs
	bool remote;	// Used to determine if player has remote control powerup, default false

	bool isFireShield;
	bool isInvincible;
	bool wallPhase;
	bool bombPhase;

	bool justDied;		// Used to determine if player just died, for checking if can respawn or should fully die
};