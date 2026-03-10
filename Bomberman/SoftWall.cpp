#include "SoftWall.h"

using namespace Constants;

SoftWall::SoftWall(const sf::Texture& tex, Pod(&pods)[_rows][_cols], int x, int y) : Entity(tex, pods)
{
	tileX = x;
	tileY = y;
	setTexture(sf::IntRect({ 64, 48 }, { _tileSize, _tileSize }));
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void SoftWall::update()
{
	if(pods[tileY][tileX].filled)		// If pod is filled, nothing to do, can skip everything
		return;

	die();			// Can safely call die() since only here if pod is empty

	// Animate
	animate();
}

void SoftWall::animate()
{
	if (state != State::Dying)	// Only animate if dying, no reason to continue
		return;

	myTick++;

	if (myTick % 3 != 0)		// Might need to change this? but seems ok to me
		return;

	if (myFrame < 6)						// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture(sf::IntRect({ myFrame * _tileSize + 64, 48 }, { _tileSize, _tileSize }));
	}

	if (myFrame >= 6)						// Once animation is finished, fully die
		state = State::Dead;
}

void SoftWall::die()
{
	if(state != State::Living)
		return;

	state = State::Dying;
	myFrame = myTick = 0;
}


// *** Public debugging method *** //

SoftWall& SoftWall::operator=(const SoftWall& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}