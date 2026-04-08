#pragma once
#include "Entity.h"
#include "Constants.h"

class Pod;

/*
	Explosion class, handles explosion animation and logic.
	Created when a bomb explodes, has a short lifespan and then dies.
	Can damage player and enemies, and destroy soft walls.
*/

class Explosion : public Entity
{
public:
	Explosion(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols],
		int, int, Facing, bool);

	void update();
	void animate();
	void die();

	friend std::ostream& operator<<(std::ostream&, const Explosion&);
	Explosion& operator=(const Explosion&);

private:
	void setTexture();

	int row;
	bool end;
	bool shrink;
};
