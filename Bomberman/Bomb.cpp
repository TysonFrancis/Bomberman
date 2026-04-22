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
	pods[tileY][tileX].isHard = false;
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

		Pod& pod = pods[yPos][xPos];

		if (xPos < 0 || xPos >= _cols ||
			yPos < 0 || yPos >= _rows )			// If out of bounds, stop checking in that direction, exit loop
			break;

		if (!pod.isFilled && !pod.isExit)			// If pod is empty,
		{
			if (d == distance)						// If at end of range, spawn end explosion
				explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, true));
			else									// Else, spawn interior explosion
				explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, false));
			continue;								// Skip to next iteration
		}

		else if (pod.isSoft)			// If a soft wall, make dying, break direction
		{
			pod.isDying = true;
			break;
		}

		else if (pod.isExit)			// If exit, explode on it with end explosion, break direction
		{
			explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, true));
			break;
		}

		else if (pod.isBomb)			// If bomb, blow it up and continute checking if more things exist
		{
			pod.isFilled = false;
			pod.isBomb = false;
			pod.isHard = false;
			explosions.emplace_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, false));
		}
		else if (pod.isHard)			//Check if it is hard wall after checking if its a bomb
			break;
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