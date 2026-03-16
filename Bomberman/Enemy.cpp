#include "Enemy.h"
#include <iostream>

using namespace Constants;

Enemy::Enemy(const sf::Texture& tex, Pod (&pods)[_rows][_cols], Type input) :
	Entity(tex, pods), type(input), speed(0.f), moveX(0), moveY(0), lastFacing(Facing::Left)
{
	switch (type)
	{
	case Type::Doria:	speed = 1.f; break;
	case Type::Ballom:
	case Type::Ovape:	speed = 2.f; break;
	case Type::Onil:
	case Type::Dahl:	speed = 3.f; break;
	case Type::Minvo:	speed = 4.f; break;
	case Type::Pass:	speed = 5.f; break;
	case Type::Pontan:  speed = 6.f; break;
	}
	
	// Set texture based on enum Type
	setTexture(sf::IntRect({ 0, 240 + static_cast<int>(type) * _tileSize}, _tile));
	changeDirection();
}

void Enemy::update()
{
	if (state == State::Dead)		// Nothing to do if dead, can skip everything
		return;

	// Movement
	if (state == State::Living)
	{
		// Update tile position based on current world position
		tileX = static_cast<int>((getSprite().getPosition().x ) / _scaledTile);
		tileY = static_cast<int>((getSprite().getPosition().y ) / _scaledTile);

		bool pause = (rand() % 9 == 0);

		if (static_cast<int>(sprite.getPosition().x) % _scaledTile != _halfScaled ||
			static_cast<int>(sprite.getPosition().y) % _scaledTile != _halfScaled)
			pause = false;

		switch (type)
		{
		case Type::Ballom: //ballom, random movement
			if (!pause)
			{
				int nextX = tileX + moveX;			// Calculate next tile position based on input to avoid 
				int nextY = tileY + moveY;			// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.

				if (moveX != 0)						// If moving horizontally,
					if (isObstructed(nextX, tileY))		// If pod in next tile is solid and colliding, stop + change direction
					{
						changeDirection();			//recovery variables say that when colliding into a wall recenter in the pod
						double recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
						moveX = recoverX/speed;
						double recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
						moveY = recoverY / speed;
					}

				if (moveY != 0)						// If moving vertically,
					if (isObstructed(tileX, nextY))		// If pod in next tile is solid and colliding, stop + change direction
					{
						changeDirection();			//recovery variables say that when colliding into a wall recenter in the pod
						double recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
						moveY = recoverY/speed;
						double recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
						moveX = recoverX / speed;
					}

				move({ (float)(moveX * speed), (float)(moveY * speed) });
			}

			else
				changeDirection();
			break;

		case Type::Onil: //onil, chases player if close
			break;

		case Type::Dahl: //dahl, random movement
			break;

		case Type::Minvo: //minvo, chases player
			break;

		case Type::Doria: //doria, chases, avoids bombs, moves through soft blocks
			break;

		case Type::Ovape: //ovape, random movement, moves through soft blocks
			break;

		case Type::Pass: //pass, always chases if encountered
			break;

		case Type::Pontan: //pontan, alwyays chases, moves through soft blocks
			break;
		}
	}

	// Animation
	animate();
}

void Enemy::animate()
{
	myTick++;								// Increment tick every update

	if(myTick % 10 != 0)					// Only update frame every 10 ticks, 60fps -> 6 frames per second
		return;

	if (dir == Facing::Left || dir == Facing::Right)
		lastFacing = dir;					// Update last facing if moving left or right, so if 
											// moving up or down, it will still show the correct
	Facing facing = lastFacing;				// texture instead of defaulting to a set direction

	if (state == State::Living)				// If alive,
	{
		myFrame = (myFrame + 1) % _moveFrames;			// Loop through frames for walking animation

		if(facing == Facing::Left)
			setTexture(sf::IntRect({ myFrame * _tileSize + 48, 240 }, _tile));
		else
			setTexture(sf::IntRect({ myFrame * _tileSize, 240 }, _tile));

		return;
	}

	if (state == State::Dying)				// If dying,
	{
		if (myTick < _fps)						// Don't continue with death animation until after 1 second
			return;

		if (myFrame < 4)						// Keep incrementing frame until finished with death animation
			myFrame++;

		if (myFrame >= 4)						// Once animation is finished, fully die
			state = State::Dead;

		int deathRow = 240;							// Deafault to pink death row
		if (type == Type::Onil || type == Type::Doria)
			deathRow = 288;							// Set to blue death row
		else if (type == Type::Dahl || type == Type::Ovape)
			deathRow = 272;							// Set to purple death row

		setTexture(sf::IntRect({ myFrame * _tileSize + 112, deathRow }, _tile));
	}
}

void Enemy::die()
{
	if(state != State::Living)
		return;

	state = State::Dying;
	myFrame = myTick = 0;
	setTexture(sf::IntRect({ 96, 240 + static_cast<int>(type) * _tileSize }, _tile));
}


// *** Private helper methods *** //

bool Enemy::isObstructed(int checkX, int checkY)
{
	if (checkX < 0 || checkX >= _cols ||		// Out of bounds, treat as solid
		checkY < 0 || checkY >= _rows)
		return true;

	// return if pod in question is solid and colliding
	return pods[checkY][checkX].isFilled && intersects(checkX, checkY);
}

void Enemy::changeDirection()
{
	dir = static_cast<Entity::Facing>(rand() % 4);

	switch (dir)
	{
	case Facing::Up:
		moveX = 0;	
		moveY = -1;		
		if(!pods[tileY-1][tileX].isFilled)
			break;
		[[fallthrough]];
	case Facing::Down:
		moveX = 0;	
		moveY = 1;		
		if (!pods[tileY+1][tileX].isFilled)
			break;
		[[fallthrough]];
	case Facing::Left:
		moveX = -1; 
		moveY = 0;		
		if (!pods[tileY][tileX-1].isFilled)
			break;
		[[fallthrough]];
	case Facing::Right:
		moveX = 1;	
		moveY = 0;		
		if (!pods[tileY][tileX+1].isFilled)
			break;
		[[fallthrough]];
	default:
		moveX = 0;
		moveY = 0;
	}
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& out, const Enemy& enemy)
{
	out << "Enemy type: ";

	switch (enemy.type)
	{
	case Enemy::Type::Ballom:	out << "Ballom";	break;
	case Enemy::Type::Onil:		out << "Onil";		break;
	case Enemy::Type::Dahl:		out << "Dahl";		break;
	case Enemy::Type::Minvo:	out << "Minvo";		break;
	case Enemy::Type::Doria:	out << "Doria";		break;
	case Enemy::Type::Ovape:	out << "Ovape";		break;
	case Enemy::Type::Pass:		out << "Pass";		break;
	case Enemy::Type::Pontan:	out << "Pontan";	break;
	}

	out << "\n";

	return out;
}

Enemy& Enemy::operator=(const Enemy& other)
{
	if (this != &other)
		Entity::operator=(other);
	return *this;
}