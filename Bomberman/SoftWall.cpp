#include "SoftWall.h"

using namespace Constants;

SoftWall::SoftWall(const Animations& frames, Pod(&pods)[_rows][_cols], int x, int y) : Entity(frames, pods)
{
	tileX = x;
	tileY = y;

	setTexture(sf::IntRect({ 64, 48 }, _tile));
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void SoftWall::update()
{
	if (pods[tileY][tileX].isSoft ||	// If pod is soft wall, or dead, return
		state == State::Dead)
		return;
					// Once here, bomb has deleted the softwall,
					// now determine if pod is the exit or regular

	if (pods[tileY][tileX].isExit)		// If pod is exit tile, set textrue to exit and fully die
		setTexture(sf::IntRect({ 176, 48 }, _tile));
	else								// Else, die
		die();

	if (state == State::Dying)		// Only animate if dying, no reason to execute
		animate();
}

void SoftWall::animate()
{
	myTick++;

	if (myTick % _wallTickSpeed != 0)			// Might need to change this timing? but seems ok to me		- D
		return;

	if (myFrame < 6)				// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture(sf::IntRect({ myFrame * _tileSize + 64, 48 }, _tile));
	}

	if (myFrame >= 6)				// Once animation is finished, fully die
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