#include "Bomb.h"

#include <iostream>

using namespace Constants;

using std::cout;

Bomb::Bomb(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	std::vector<Explosion>& explosions, bool timer, int d, int x, int y) :
		Entity(tex, pods), explosions(explosions), remote(timer), distance(d), shrink(false)
{
	myFrame = 2;
	tileX = x;
	tileY = y;

	setTexture(sf::IntRect({ 32, 48 }, _tile));
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void Bomb::update()
{
	if (myTick >= _fps * _bombTimer && !remote)	// If 2.5 seconds and no remote explode
		explode();
	if (willExplode && now + 5 <= myTick)
		explode();

	animate();
}

void Bomb::animate()
{
	myTick++;

	if (myTick % _bombTickSpeed != 0)
		return;

	if (state == State::Living)					// Alive animations
	{
		myFrame += shrink ? 1 : -1;

		if (myFrame <= 0)							// If at largest size, shrink
		{
			//myFrame = 0;
			shrink = true;
		}
		else if (myFrame >= _bombFrames - 1)		// If at smallest size, enlarge
		{
			//myFrame = _bombFrames - 1;
			shrink = false;
		}

		if (myFrame < 0 || myFrame >= _bombFrames)
		{
			//cout << "Bomb frame OOB: " << myFrame << "\n";
			return;
		}

		setTexture(sf::IntRect({ myFrame * _tileSize, 48 }, _tile));

		return;
	}

	// Death animation
	if (myFrame < 4)							// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture(sf::IntRect({ myFrame * _tileSize, 48 }, _tile));
	}

	else										// Set to empty texture to let background through after fully dies
		setTexture(_emptyFrame);
}

void Bomb::delay()
{
	willExplode = true;
	now = myTick;
}

bool Bomb::getWillExplode()
{
	return willExplode;
}


void Bomb::explode()
{
	state = State::Dead;
	pods[tileY][tileX].isFilled = false;
	pods[tileY][tileX].isBomb = false;
	myFrame = myTick = 0;
	explosions.push_back(Explosion(sprite.getTexture(), pods, tileX, tileY, dir, false));

	// Need to make better selection than this for direction	- D
	dir = Facing::Up;
	propogate(0, -1);		// Checks up
	dir = Facing::Down;
	propogate(0, 1);		// Checks down
	dir = Facing::Left;
	propogate(-1, 0);		// Checks left
	dir = Facing::Right;
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
			yPos < 0 || yPos >= _rows ||
			pods[yPos][xPos].isHard)			// If out of bounds or hard wall, stop checking in that direction, exit loop
			break;

		if (!pods[yPos][xPos].isFilled)			// If pod is empty,
		{
			if (d == distance)						// If at end of range, spawn end explosion
				explosions.push_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, true));
			else									// Else, spawn interior explosion
				explosions.push_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, false));
			/*for (int i = 0; i < explosions.size(); i++)
			{
				if(explosions[i].tileX==Xpos)
			}*/
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
			explosions.push_back(Explosion(sprite.getTexture(), pods, xPos, yPos, dir, false));
		}
	}
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& os, const Bomb& bomb)
{
	return os << "Position: (" << bomb.tileX << ", " << bomb.tileY << ")\t"
		<< "frame: " << bomb.myFrame << "\n";
}

Bomb& Bomb::operator=(const Bomb& other)
{
	if (this != &other)
		Entity::operator=(other);
	return *this;
}