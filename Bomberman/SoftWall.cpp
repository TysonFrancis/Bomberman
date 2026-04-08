#include "SoftWall.h"
#include "Pod.h"
#include <iostream>

using namespace Constants;
using std::cout, std::endl;

SoftWall::SoftWall(const sf::Texture& tex, Pod(&pods)[_rows][_cols], int x, int y) :
	Entity(tex, pods)
{
	setTexture(_softWallX, _softWallY);
	setPosition(x, y);
}

void SoftWall::update()
{
	if (pods[tileY][tileX].isSoft ||	// If pod is soft wall, or dead, return
		state == State::Dead)
		return;
					// Once here, bomb has deleted the softwall,
					// now determine if pod is the exit or regular

	if (pods[tileY][tileX].isExit)		// If pod is exit tile, set textrue to exit
		setTexture(_exitX, _exitY);
	else								// Else, die
		die();

	if (state == State::Dying)			// Only animate if dying, no reason to execute
		animate();
}

void SoftWall::animate()
{
	tick++;

	if (tick % _wallTickSpeed != 0)			// Leave method if not time to update frame yet
		return;

	if (frame < _softWallDeathFrames)			// Keep incrementing frame until finished with death animation
	{
		frame++;
		setTexture(frame * _tileSize + _softWallX, _softWallY);
	}

	if (frame >= _softWallDeathFrames)		// Once animation is finished, fully die
		state = State::Dead;
}

void SoftWall::die()
{
	if(state != State::Living)
		return;

	state = State::Dying;
	frame = tick = 0;
}


// *** Public debugging method *** //

SoftWall& SoftWall::operator=(const SoftWall& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}