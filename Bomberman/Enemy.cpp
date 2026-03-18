#include "Enemy.h"
#include <iostream>

using namespace Constants;

Enemy::Enemy(const sf::Texture& tex, Pod (&pods)[_rows][_cols], Type input,Player(&play)) :
	Entity(tex, pods), type(input), speed(0.f), moveX(0), moveY(0), lastFacing(Facing::Left),play(play)
{
	switch (type)
	{
	case Type::Doria:	speed = 1.5f; eratic = 11; break;
	case Type::Ballom:
	case Type::Ovape:	speed = 2.f; eratic = 9;  break;
	case Type::Onil:
	case Type::Dahl:	speed = 2.5f; eratic = 5; break;
	case Type::Minvo:	speed = 3.f; eratic = 4; break;
	case Type::Pass:	speed = 3.5f; eratic = 3; break;
	case Type::Pontan:  speed = 4.f; eratic = 5; break;
	}
	
	// Set texture based on enum Type
	setTexture(sf::IntRect({ 0, 240 + static_cast<int>(type) * _tileSize}, _tile));
	changeDirection(false);
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

		

		switch (type)
		{
		case Type::Ballom: //ballom, random movement
			randomMove(false);
			break;

		case Type::Onil: //onil, chases player if seen on y axis
			chasePlayer(false, true, false);
			break;

		case Type::Dahl: //dahl, chases player if seen on X axis
			chasePlayer(true, false,false);
			break;

		case Type::Minvo: //minvo, chases player if seen on either axis
			chasePlayer(true, true,false);
			break;

		case Type::Doria: //doria, chases, avoids bombs, moves through soft blocks
			chasePlayer(true, true, true);
			break;

		case Type::Ovape: //ovape, random movement, moves through soft blocks
			randomMove(true);
			break;

		case Type::Pass: //pass, always chases if encountered, changes direction at most junctions
			chasePlayer(true, true,true);
			break;

		case Type::Pontan: //pontan, alwyays chases, moves through soft blocks
			chasePlayer(true, true,true);
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
			setTexture(sf::IntRect({ myFrame * _tileSize + 48, 240 + static_cast<int>(type)  * _tileSize}, _tile));
		else
			setTexture(sf::IntRect({ myFrame * _tileSize, 240 + static_cast<int>(type) * _tileSize }, _tile));

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

bool Enemy::isObstructed(int checkX, int checkY,bool phase)
{
	if (checkX < 0 || checkX >= _cols ||		// Out of bounds, treat as solid
		checkY < 0 || checkY >= _rows)
		return true;

	// return if pod in question is solid and colliding
	if(!phase)
		return pods[checkY][checkX].filled && intersects(checkX, checkY);
	return pods[checkY][checkX].isHard && intersects(checkX, checkY);
}

void Enemy::changeDirection(bool phase)
{
	dir = static_cast<Entity::Facing>(rand() % 4);

	switch (dir)
	{
	case Facing::Up:
		moveX = 0;	
		moveY = -1;		
		if(!pods[tileY-1][tileX].filled)
			break;
		[[fallthrough]];
	case Facing::Down:
		moveX = 0;	
		moveY = 1;		
		if (!phase&&!pods[tileY+1][tileX].filled)
			break;
		if (phase && !pods[tileY + 1][tileX].isHard)
			break;
		[[fallthrough]];
	case Facing::Left:
		moveX = -1; 
		moveY = 0;		
		if (!phase&&!pods[tileY][tileX-1].filled)
			break;
		if (phase && !pods[tileY][tileX - 1].isHard)
			break;
		[[fallthrough]];
	case Facing::Right:
		moveX = 1;	
		moveY = 0;		
		if (!phase&&!pods[tileY][tileX+1].filled)
			break;
		if (phase && !pods[tileY][tileX + 1].isHard)
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

void Enemy::randomMove(bool canPhase)
{
	bool paused = (rand() % eratic == 0);

	if (static_cast<int>(sprite.getPosition().x) % _scaledTile != _halfScaled ||
		static_cast<int>(sprite.getPosition().y) % _scaledTile != _halfScaled)
		paused = false;

	if (!paused)
	{
		int nextX = tileX + moveX;			// Calculate next tile position based on input to avoid 
		int nextY = tileY + moveY;			// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.

		if (moveX != 0)						// If moving horizontally,
			if (isObstructed(nextX, tileY,canPhase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(canPhase);			//recovery variables say that when colliding into a wall recenter in the pod
				double recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
				moveX = recoverX / speed;
				double recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
				moveY = recoverY / speed;
			}

		if (moveY != 0)						// If moving vertically,
			if (isObstructed(tileX, nextY,canPhase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(canPhase);			//recovery variables say that when colliding into a wall recenter in the pod
				double recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
				moveY = recoverY / speed;
				double recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
				moveX = recoverX / speed;
			}

		move({ (float)(moveX * speed), (float)(moveY * speed) });

	}
	else
		changeDirection(canPhase);
}

void Enemy::chasePlayer(bool x, bool y,bool phase)
{
	if (static_cast<int>(sprite.getPosition().x) % _scaledTile != _halfScaled ||
		static_cast<int>(sprite.getPosition().y) % _scaledTile != _halfScaled)
	{
		randomMove(phase);
		return;
	}
	bool found = false;
	if (x)
	{
		if (tileY == play.getY() && lineOfSight(false, phase))
		{
			if (tileX - play.getX() > 0)
				moveX = -1;
			else
				moveX = 1;
			moveY = 0;
			found = true;
		}
	}
	if (y)
	{
		if (tileX == play.getX()&&lineOfSight(true,phase))
		{
			if (tileY - play.getY() > 0)
				moveY = -1;
			else
				moveY = 1;
			moveX = 0;
			found = true;
		}
	}
	if (!found)
		randomMove(phase);
	else
	{
		int nextX = tileX + moveX;			// Calculate next tile position based on input to avoid 
		int nextY = tileY + moveY;			// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.

		if (moveX != 0)						// If moving horizontally,
			if (isObstructed(nextX, tileY, phase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(phase);			//recovery variables say that when colliding into a wall recenter in the pod
				double recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
				moveX = recoverX / speed;
				double recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
				moveY = recoverY / speed;
			}

		if (moveY != 0)						// If moving vertically,
			if (isObstructed(tileX, nextY, phase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(phase);			//recovery variables say that when colliding into a wall recenter in the pod
				double recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
				moveY = recoverY / speed;
				double recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
				moveX = recoverX / speed;
			}

		move({ (float)(moveX * speed), (float)(moveY * speed) });
	}
}

bool Enemy::lineOfSight(bool xy, bool phase)	//xy-0means x direction, 1 means y
{
	return true;
}