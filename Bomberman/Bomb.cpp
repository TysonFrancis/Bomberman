#include "Bomb.h"
#include "Explosion.h"
#include "Pod.h"
#include <iostream>

using namespace Constants;
using std::cout, std::endl;

Bomb::Bomb(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	std::vector<Explosion>& explosions, bool timer, int d, int x, int y) :
		Entity(tex, pods), explosions(explosions), distance(d),
		remote(timer), shrink(false), now(0), willExplode(false)
{
	frame = 2;

	setTexture(32, 48);
	setPosition(x, y);
}

void Bomb::update()
{
	if (tick >= _fps * _bombTimer && !remote)	// If 2.5 seconds and no remote explode
		die();
	if (willExplode && now + _bombDelay <= tick)
		die();

	animate();
}

void Bomb::animate()
{
	tick++;

	if (tick % _bombTickSpeed != 0)
		return;

	if (state == State::Living)					// Alive animations
	{
		frame += shrink ? 1 : -1;

		if (frame < 0 || frame >= _bombFrames)	// If frame out of bounds leave method
			return;

		if (frame <= 0)							// If at largest size, shrink
			shrink = true;
		else if (frame >= _bombFrames - 1)		// If at smallest size, enlarge
			shrink = false;

		setTexture(frame * _tileSize, _bombY);

		return;
	}

	// Death animation
	if (frame <= _bombFrames)						// Keep incrementing frame until finished with death animation
	{
		frame++;
		setTexture(frame * _tileSize, _bombY);
	}
}

void Bomb::die()
{
	state = State::Dead;
	pods[tileY][tileX].isFilled = false;
	pods[tileY][tileX].isBomb = false;
	frame = tick = 0;
	explosions.emplace_back(Explosion(sprite.getTexture(), pods, tileX, tileY, dir, false));

	propogate(0, -1, Facing::Up);		// Checks up
	propogate(0, 1, Facing::Down);		// Checks down
	propogate(-1, 0, Facing::Left);		// Checks left
	propogate(1, 0, Facing::Right);		// Checks right
}

void Bomb::delay()					{ willExplode = true; now = tick; }
bool Bomb::getWillExplode() const	{ return willExplode; }


// *** Private helper method *** //

void Bomb::propogate(int xDir, int yDir, Facing dir)
{
	for (int d = 1; d <= distance; d++)
	{
		int xPos = tileX + xDir * d;			// Resized postion to check based on direction and distance
		int yPos = tileY + yDir * d;

		if (xPos < 0 || xPos >= _cols ||
			yPos < 0 || yPos >= _rows ||
			pods[yPos][xPos].isHard)			// If out of bounds or hard wall, stop checking in that direction, exit loop
			break;

		if (!pods[yPos][xPos].isFilled)			// If pod is empty,
		{
			if (d == distance)						// If at end of range, spawn end explosion
				explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, true));
			else									// Else, spawn interior explosion
				explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, false));
			continue;								// Skip to next iteration
		}

		else if (pods[yPos][xPos].isSoft)			// If a soft wall is in the way,
		{
			pods[yPos][xPos].isFilled = false;
			pods[yPos][xPos].isSoft = false;
			break;								// Since loop continues if empty, if it reaches this point, it
												// means it's a soft wall that was just destroyed, so exit loop
		}
		
		else if (pods[yPos][xPos].isBomb)
		{
			pods[yPos][xPos].isFilled = false;
			pods[yPos][xPos].isBomb = false;
			explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, false));
		}
	}
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& os, const Bomb& bomb)
{
	return os << "Position: (" << bomb.tileX << ", " << bomb.tileY << ")\t"
		<< "frame: " << bomb.frame << "\n";
}

Bomb& Bomb::operator=(const Bomb& other)
{
	if (this != &other)
	{
		Entity::operator=(other);

		distance = other.distance;
		remote = other.remote;
		shrink = other.shrink;
		now = other.now;
		willExplode = other.willExplode;
	}

	return *this;
}