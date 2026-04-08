#pragma once
#include "Entity.h"
#include "Constants.h"

class Explosion;
class Pod;

/*
	Bomb class, handles bombanimation and explosion propogation.
	Created when player spawns a bomb,
	has a short lifespan and then explodes.
*/

class Bomb : public Entity
{
public:
	Bomb(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols],
		std::vector<Explosion>&, bool, int, int, int);

	void update();
	void animate();
	void die();

	void delay();
	bool getWillExplode() const;

	friend std::ostream& operator<<(std::ostream&, const Bomb&);
	Bomb& operator=(const Bomb&);
private:
	void propogate(int, int, Facing);

	std::vector<Explosion>& explosions;

	int distance;
	bool remote;		// Checks if player has remote power up
	bool shrink;
	int now;			// Used to delay explosion 
	bool willExplode;
};