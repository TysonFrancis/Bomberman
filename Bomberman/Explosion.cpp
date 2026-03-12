#include "Explosion.h"

#include <iostream>

using namespace Constants;

Explosion::Explosion(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	int x, int y, Facing dir, bool isEnd) : Entity(tex, pods), row(1), end(isEnd)
{
	tileX = x;
	tileY = y;
	this->dir = dir;

	setTexture();
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void Explosion::update()
{
	if (state == State::Living)		// Only thing to do is animate, and only does that when alive
		animate();
}

void Explosion::animate()
{
	myTick++;

	if (myTick % 15 != 0)		// Same speed to match with soft wall dying, need to look at emulator 
		return;

	if (myFrame >= 1)			// Once frame advances past 1, switch to second row explosions
		row = 2;

	if (myFrame < 4)			// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture();
	}

	if (myFrame >= 4)			// Once animation is finished, fully die
		state = State::Dead;
}


// *** Private helper method *** //

void Explosion::setTexture()
{
	// Helper variables to reduce redundant code
	int leftX, rightX, baseX, upY, downY, baseY;

	// Base location setup
	baseX = myFrame * _explosionOffset + _explosionStartX;
	baseY = _explosionStartY;

	if (row >= 2)					// If on row two,
	{									// offset the frame count by 2 for BaseX + move down baseY
		baseX = (myFrame - 2) * _explosionOffset + _explosionStartX;
		baseY += _explosionOffset;
	}

	// Specific location setup after row check
	leftX = baseX - _tileSize;
	rightX = baseX + _tileSize;
	upY = baseY - _tileSize;
	downY = baseY + _tileSize;

	if (end)						// If edge explosion,
	{									// offset directions by a tile size
		leftX -= _tileSize;
		rightX += _tileSize;
		upY -= _tileSize;
		downY += _tileSize;
	}

	switch (dir)					// Apply selected texture
	{
	case Facing::Up:	Entity::setTexture(sf::IntRect({ baseX,  upY   }, _tile));	break;
	case Facing::Down:	Entity::setTexture(sf::IntRect({ baseX,  downY }, _tile));	break;
	case Facing::Left:	Entity::setTexture(sf::IntRect({ leftX,  baseY }, _tile));	break;
	case Facing::Right:	Entity::setTexture(sf::IntRect({ rightX, baseY }, _tile));	break;
	case Facing::None:	Entity::setTexture(sf::IntRect({ baseX,	 baseY }, _tile));	break;
	}
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& os, const Explosion& explosion)
{
	if(explosion.end)
		os << "Position: (" << explosion.tileX << ", " << explosion.tileY << ")\t"
			<< "frame: " << explosion.myFrame << "\trow: " << explosion.row
			<< "\tend: " << (explosion.end ? "true" : "false") << "\n";
	else
		os << "Not end\n";

	return os;
}

Explosion& Explosion::operator=(const Explosion& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}