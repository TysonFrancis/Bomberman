#include "Explosion.h"

#include <iostream>

using namespace Constants;

Explosion::Explosion(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	int x, int y, Facing dir, bool isEnd, const Animations& frames) :
		Entity(tex, pods), row(1), end(isEnd), shrink(false), frames(frames)
{
	tileX = x;
	tileY = y;
	this->dir = dir;
	type = setType(dir);

	//setTexture();
	Entity::setTexture(frames.getExplosion(type)[0]);
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void Explosion::update()
{
	if (myTick >= _explosionTickSpeed * 6)		// If been living for 7 or more frames
	{
		die();
	}

	animate();
}

void Explosion::animate()
{
	myTick++;

	if (myTick % _explosionTickSpeed != 0)
		return;

	if (state == State::Living)
	{
		if (shrink)						// If it should get smaller, frame--,
			myFrame--;
		else							// Otherwise, frame++
			myFrame++;

		if (myFrame <= 0)				// If at smallest size, enlarge
		{
			myFrame = 0;
			shrink = false;
		}

		else if (myFrame >= 3)			// If at largest size, shrink
		{
			myFrame = 3;
			shrink = true;
		}

		if (myFrame > 1)				// If frame advances past 1, switch to second row explosions
			row = 2;
		if (myFrame < 2 && shrink)		// If frame is less than 2 and it should get smaller, switch to first row
			row = 1;

		// Make a local reference to the explosion frames vector based on explosion type
		// auto -> std::vector<sf::IntRect>
		const auto& explosionTextures = frames.getExplosion(type);

		if (myFrame < 0 || myFrame >= explosionTextures.size())
		{
			std::cout << "Frame OOB: " << myFrame << " size: " << explosionTextures.size() << "\n";
			return;
		}

		// Set texture based on explosion type and current
		// frame, using the explosion frame hash table
		Entity::setTexture(explosionTextures[myFrame]);

		//setTexture();

		return;
	}

	if (myFrame <= 0 && state == State::Dying)
		state = State::Dead;
}

void Explosion::die()
{
	state = State::Dying;
}


// *** Private helper methods *** //

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

// Determines explosion type based on direction and if it's an
// end explosion or not, used as key for explosion frame hash table
ExplosionType Explosion::setType(Facing dir) const
{
	if (dir == Facing::Up)
		return end ? ExplosionType::EndUp : ExplosionType::Up;
	else if (dir == Facing::Down)
		return end ? ExplosionType::EndDown : ExplosionType::Down;
	else if (dir == Facing::Left)
		return end ? ExplosionType::EndLeft : ExplosionType::Left;
	else if (dir == Facing::Right)
		return end ? ExplosionType::EndRight : ExplosionType::Right;
	else
		return ExplosionType::Center;
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& os, const Explosion& explosion)
{
	os << "Position: (" << explosion.tileX << ", " << explosion.tileY << ")\t"
		<< "frame: " << explosion.myFrame; /*<< "\trow: " << explosion.row
		<< "\tend: " << (explosion.end ? "true" : "false")
		<< "\tstate: ";

	switch (explosion.state)
	{
	case Entity::State::Living:	std::cout << "living";	break;
	case Entity::State::Dying:	std::cout << "dying";	break;
	case Entity::State::Dead:	std::cout << "dead";	break;
	}*/

	std::cout << "\n";

	return os;
}

Explosion& Explosion::operator=(const Explosion& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}