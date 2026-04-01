#include "Explosion.h"

#include <iostream>

using namespace Constants;

using std::cout, std::endl;

Explosion::Explosion(const sf::Texture& tex, Pod(&pods)[_rows][_cols],
	int x, int y, Facing dir, bool isEnd) :
		Entity(tex, pods), row(1), end(isEnd), shrink(false)
{
	this->dir = dir;

	setTexture();
	setPosition(x, y);
}

void Explosion::update()
{
	//cout << *this;
	if (myTick >= _explosionTickSpeed * 6)		// If been living for 7 or more frames
		die();

	animate();
}

void Explosion::animate()
{
	myTick++;

	if (myTick % _explosionTickSpeed != 0)
		return;

	if (state == State::Living)
	{
		myFrame += shrink ? -1 : 1;

		if (myFrame <= 0)							// If at smallest size, enlarge
		{
			//myFrame = 0;
			shrink = false;
		}

		else if (myFrame >= _explosionFrames - 1)	// If at largest size, shrink
		{
			//myFrame = 3;
			shrink = true;
		}

		if (myFrame > 1)				// If frame advances past 1, switch to second row explosions
			row = 2;
		if (myFrame < 2 && shrink)		// If frame is less than 2 and it should get smaller, switch to first row
			row = 1;

		if (myFrame < 0 || myFrame >= _explosionFrames)
		{
			//cout << "Explosion frame OOB: " << myFrame << "\n";
			return;
		}

		setTexture();

		return;
	}

	if (myFrame <= 0 && state == State::Dying)
		state = State::Dead;
}

void Explosion::die()
{
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
	case Facing::Up:	Entity::setTexture(baseX,  upY  );	break;
	case Facing::Down:	Entity::setTexture(baseX,  downY);	break;
	case Facing::Left:	Entity::setTexture(leftX,  baseY);	break;
	case Facing::Right:	Entity::setTexture(rightX, baseY);	break;
	case Facing::None:	Entity::setTexture(baseX,  baseY);	break;
	}

	/*if(dir == Facing::None)
		cout << "\tbaseX: " << baseX << "\tbaseY: " << baseY << "\t";*/
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& out, const Explosion& explosion)
{
	if(explosion.dir == Entity::Facing::None)
	{
		out << "Position: (" << explosion.tileX << ", " << explosion.tileY << ")\t"
			<< "frame: " << explosion.myFrame << "\trow: " << explosion.row
			<< "\tshrink: " << std::boolalpha << explosion.shrink;
			/* << "\tend: " << (explosion.end ? "true" : "false")
			<< "\tstate: ";

		switch (explosion.state)
		{
		case Entity::State::Living:	cout << "living";	break;
		case Entity::State::Dying:	cout << "dying";	break;
		case Entity::State::Dead:	cout << "dead";	break;
		}*/

		out << "\t";
	}

	return out;
}

Explosion& Explosion::operator=(const Explosion& other)
{
	if (this != &other)
		Entity::operator=(other);

	return *this;
}