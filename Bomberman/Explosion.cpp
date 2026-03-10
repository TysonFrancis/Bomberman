#include "Explosion.h"

Explosion::Explosion(const sf::Texture& tex, Pod(&pods)[_rows][_cols], int x, int y, Facing dir) :
	Entity(tex, pods)
{
	tileX = x;
	tileY = y;
	this.dir = dir;
}

void Explosion::update()
{
	// Animation
	animate();
}

void Explosion::animate()
{
	if (state != State::Dying)
		return;

	myTick++;

	if (myTick % 3 != 0)
		return;
}

void Explosion::propogate()
{

}


// *** Public debugging method *** //

Explosion& Explosion::operator=(const Explosion& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}