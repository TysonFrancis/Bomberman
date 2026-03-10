#include "Bomb.h"
#include <iostream>

using namespace Constants;

Bomb::Bomb(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	bool timer, int d, int x, int y) :
	Entity(tex, pods), remote(timer), distance(d), up(false)
{
	myFrame = 2;
	tileX = x;
	tileY = y;
	setTexture(sf::IntRect({ 0, 48 }, { _tileSize, _tileSize }));
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void Bomb::update()
{
	if (myTick >= _fps * 3 && !remote)	// If 3 seconds and no remote explode
		explode();

	animate();
}

void Bomb::animate()
{
	myTick++;

	if (myTick % 15 != 0)						// Changes size every 15 ticks
		return;

	if (state == State::Living)					// Alive animations
	{
		if (up)
			myFrame++;
		else
			myFrame--;

		if (myFrame == 0)
			up = true;
		else if (myFrame == 2)
			up = false;

		setTexture(sf::IntRect({ myFrame * 16, 48 }, { _tileSize, _tileSize }));

		return;
	}

	// Death animation
	if (myFrame < 4)							// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture(sf::IntRect({ myFrame * 16, 48 }, { _tileSize, _tileSize }));
	}

	else										// Set to empty texture to let background through after fully dies
		setTexture(_emptyFrame);
}

void Bomb::explode()
{
	state = State::Dead;
	pods[tileY][tileX].filled = false;

	propogate(0, -1);		// Checks up
	propogate(0, 1);		// Checks down
	propogate(-1, 0);		// Checks left
	propogate(1, 0);		// Checks right
}


// *** Private helper method *** //

void Bomb::propogate(int xDir, int yDir)
{
	for (int d = 1; d <= distance; d++)
	{
		int xPos = tileX + xDir * d;			// Resized postion to check based on direction and distance
		int yPos = tileY + yDir * d;

		if (xPos < 0 || xPos >= _cols ||
			yPos < 0 || yPos >= _rows)			// If out of bounds, stop checking in that direction, exit loop
			break;

		if (!pods[yPos][xPos].filled)			// Skip to next check if empty tile
			continue;		// This skips to next loop iteration
		
		if (pods[yPos][xPos].isSoft)			// If a soft wall is in the way,
		{										// delete it and stop checking in that direction
			pods[yPos][xPos].filled = false;
			pods[yPos][xPos].isSoft = false;
		}

		break;			// Since loop continues if empty, if it reaches this point,
						// it means it's a hard wall, or a soft wall that was just
						// destroyed, so stop checking in that direction and exit loop
	}
}


// *** Public debugging method *** //

Bomb& Bomb::operator=(const Bomb& other)
{
	if (this != &other)
		Entity::operator=(other);
	return *this;
}