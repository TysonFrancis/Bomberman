#include "Explosion.h"

#include <iostream>

using namespace Constants;

Explosion::Explosion(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	int x, int y, Facing dir, bool isEnd) : Entity(tex, pods), row(1), end(isEnd)
{
	tileX = x;
	tileY = y;
	this->dir = dir;
	state = State::Dying;

	setTexture(dir);
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void Explosion::update()
{
	if (state == State::Dying)
		animate();
}

void Explosion::animate()
{
	myTick++;

	if (myTick % 15 != 0)
		return;

	if (myFrame == 1)
		row++;

	if (myFrame < 4)				// Keep incrementing frame until finished with death animation
	{
		myFrame++;
		setTexture(dir);
	}

	if (myFrame >= 4)				// Once animation is finished, fully die
		state = State::Dead;
}


// *** Private helper method *** //

void Explosion::setTexture(Facing dir)
{
	int newFrame;

	if (row == 1)
	{
		if (!end)
		{
			switch (dir)
			{
			case Facing::Up:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 32, 80 }, _tile));	break;
			case Facing::Down:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 32, 112 }, _tile));	break;
			case Facing::Left:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 16, 96 }, _tile));	break;
			case Facing::Right:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 48, 96 }, _tile));	break;
			case Facing::None:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 32, 96 }, _tile));	break;
			}
		}

		else
		{
			switch (dir)
			{
			case Facing::Up:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 32, 64 }, _tile));	break;
			case Facing::Down:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 32, 128 }, _tile));	break;
			case Facing::Left:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 16, 80 }, _tile));	break;
			case Facing::Right:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 48, 112 }, _tile));	break;
			case Facing::None:	Entity::setTexture(sf::IntRect({ myFrame * _explosionOffset + 32, 96 }, _tile));	break;
			}
		}
	}
	else
	{
		newFrame = myFrame - 2;

		if (!end)
		{
			switch (dir)
			{
			case Facing::Up:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 32, 160 }, _tile));	break;
			case Facing::Down:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 32, 192 }, _tile));	break;
			case Facing::Left:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 16, 176 }, _tile));	break;
			case Facing::Right:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 48, 176 }, _tile));	break;
			case Facing::None:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 32, 176 }, _tile));	break;
			}
		}

		else
		{
			switch (dir)
			{
			case Facing::Up:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 32, 144 }, _tile));	break;
			case Facing::Down:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 32, 208 }, _tile));	break;
			case Facing::Left:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 16, 160 }, _tile));	break;
			case Facing::Right:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 48, 192 }, _tile));	break;
			case Facing::None:	Entity::setTexture(sf::IntRect({ newFrame * _explosionOffset + 32, 176 }, _tile));	break;
			}
		}
	}
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& os, const Explosion& explosion)
{
	return os << "Position: (" << explosion.tileX << ", " << explosion.tileY << ")\t"
		<< "frame: " << explosion.myFrame << "\trow: " << explosion.row << "\n";
}

Explosion& Explosion::operator=(const Explosion& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}