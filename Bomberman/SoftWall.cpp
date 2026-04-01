#include "SoftWall.h"

using namespace Constants;

//using std::cout, std::endl;

SoftWall::SoftWall(const sf::Texture& tex, Pod(&pods)[_rows][_cols], int x, int y) :
	Entity(tex, pods)
{
	setTexture(_softWallX, _softWallY);
	setPosition(x, y);
}

void SoftWall::update()
{
	if (pods[tileY][tileX].isSoft ||	// If pod is soft wall, or dead, or exit, return
		state == State::Dead || state == State::Exit)
		return;
					// Once here, bomb has deleted the softwall,
					// now determine if pod is the exit or regular

	if (pods[tileY][tileX].isExit)		// If pod is exit tile, set textrue to exit and fully die
	{
		setTexture(_exitX, _exitY);
		state = State::Exit;
	}
	else								// Else, die
		die();

	if (state == State::Dying)			// Only animate if dying, no reason to execute
		animate();
}

void SoftWall::animate()
{
	myTick++;

	if (myTick % _wallTickSpeed != 0)			// Might need to change this timing? but seems ok to me		- D
		return;

	if (myFrame < _softWallDeathFrames)			// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture(myFrame * _tileSize + _softWallX, _softWallY);
	}

	if (myFrame >= _softWallDeathFrames)		// Once animation is finished, fully die
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