#include "Enemy.h"
#include "Player.h"
#include "Pod.h"
#include <iostream>

using namespace Constants;
using std::cout, std::endl;

Enemy::Enemy(const sf::Texture& tex, Pod(&pods)[_rows][_cols], Type input, Player(&play)) :
	Entity(tex, pods), play(play),
	type(input), lastFacing(Facing::Left),
	moveX(0.f), moveY(0.f),
	enemyFrameYPos(static_cast<int>(type) * _tileSize + _enemyStartY)
{
	switch (type)
	{
	case Type::Ballom:	speed = 1.5f * _speedScale;	eratic = 11;	break;
	case Type::Onil:	speed = 2.f * _speedScale;	eratic = 5;		break;
	case Type::Dahl:	speed = 2.f * _speedScale;	eratic = 5;		break;
	case Type::Minvo:	speed = 2.f * _speedScale;	eratic = 4;		break;
	case Type::Doria:	speed = 1.f * _speedScale;	eratic = 11;	break;
	case Type::Ovape:	speed = 1.5f * _speedScale;	eratic = 9;		break;
	case Type::Pass:	speed = 2.5f * _speedScale;	eratic = 3;		break;
	case Type::Pontan:	speed = 2.5f * _speedScale;	eratic = 5;		break;
	}

	setTexture(0, enemyFrameYPos);		// Set texture based on enum Type
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
		tileX = static_cast<int>((getSprite().getPosition().x) / _scaledTile);
		tileY = static_cast<int>((getSprite().getPosition().y) / _scaledTile);

		switch (type)
		{
		case Type::Ballom:  randomMove(false);					break;  // Ballom,	random movement
		case Type::Onil:    chasePlayer(false, true, false);	break;  // Onil,	chases player if seen on y axis
		case Type::Dahl:    chasePlayer(true, false, false);	break;  // Dahl,	chases player if seen on X axis
		case Type::Minvo:   chasePlayer(true, true, false);		break;  // Minvo,	chases player if seen on either axis
		case Type::Doria:   chasePlayer(true, true, true);		break;  // Doria,	chases, avoids bombs, moves through soft blocks
		case Type::Ovape:   randomMove(true);					break;  // Ovape,	random movement, moves through soft blocks
		case Type::Pass:    chasePlayer(true, true, true);		break;	// Pass,	always chases if encountered, changes direction at most junctions
		case Type::Pontan:	chasePlayer(true, true, true);		break;  // Pontan,	alwyays chases, moves through soft blocks
		}
	}

	animate();
}

void Enemy::animate()
{
	tick++;

	if (tick % _enemyTickSpeed != 0)
		return;

	if (dir == Facing::Left || dir == Facing::Right)
		lastFacing = dir;					// Update last facing if moving left or right, so if 
											// moving up or down, it will still show the correct
	Facing facing = lastFacing;				// texture instead of defaulting to a set direction

	if (state == State::Living)				// If alive,
	{
		if(type == Type::Pontan)					// Pontan has 4 walk frames and doesn't care about
		{											// last direction facing, logic has to be separate
			frame = (frame + 1) % (_moveFrames + 1);

			setTexture(frame * _tileSize, enemyFrameYPos);
			return;
		}

		frame = (frame + 1) % _moveFrames;		// Loop through frames for walking animation

		if (facing == Facing::Left)
			setTexture(frame * _tileSize + _enemyLeftX, enemyFrameYPos);
		else
			setTexture(frame * _tileSize, enemyFrameYPos);

		return;
	}

	if (state == State::Dying)				// If dying,
	{
		if (tick < _fps)						// Don't continue with death animation until after 1 second
			return;

		if (frame < _enemyColorDeathFrames)		// Keep incrementing frame until finished with death animation
			frame++;

		if (frame >= _enemyColorDeathFrames)	// Once animation is finished, fully die
			state = State::Dead;

		int deathRow = _enemyStartY;				// Deafault to pink death row
		if (type == Type::Onil || type == Type::Doria)
			deathRow += 48;							// Set to blue death row
		else if (type == Type::Dahl || type == Type::Ovape)
			deathRow += 32;							// Set to purple death row

		setTexture(frame * _tileSize + _enemyColorDeathX, deathRow);
	}
}

void Enemy::die()
{
	if (state != State::Living)
		return;

	state = State::Dying;
	frame = tick = 0;
	setTexture(_enemyDeathX, enemyFrameYPos);
}

Enemy::Type Enemy::getType() const	{ return type; }


// *** Private helper methods *** //

void Enemy::randomMove(bool canPhase)
{
	bool paused = (rand() % eratic == 0);

	if (static_cast<int>(sprite.getPosition().x) % _scaledTile != _halfScaled ||
		static_cast<int>(sprite.getPosition().y) % _scaledTile != _halfScaled)
		paused = false;

	if (!paused)
	{
		int nextX = static_cast<int>(tileX + moveX);			// Calculate next tile position based on input to avoid 
		int nextY = static_cast<int>(tileY + moveY);			// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.
		float tempX = moveX;
		float tempY = moveY;

		if (moveX != 0)						// If moving horizontally,
			if (isObstructed(nextX, tileY, canPhase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(canPhase);			//recovery variables say that when colliding into a wall recenter in the pod
				tempX = moveX;
				tempY = moveY;
				if (!isObstructed(tileX, tileY, canPhase))
				{
					float recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
					moveX = recoverX / speed;
					float recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
					moveY = recoverY / speed;
				}
			}

		if (moveY != 0)						// If moving vertically,
			if (isObstructed(tileX, nextY, canPhase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(canPhase);			//recovery variables say that when colliding into a wall recenter in the pod
				tempX = moveX;
				tempY = moveY;
				if (!isObstructed(tileX, tileY, canPhase))
				{
					float recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
					moveY = recoverY / speed;
					float recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
					moveX = recoverX / speed;
				}
			}

		move(moveX * speed, moveY * speed);

		moveX = tempX;
		moveY = tempY;
	}
	else
		changeDirection(canPhase);
}

void Enemy::chasePlayer(bool x, bool y, bool phase)
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
		if (tileY == play.getY() && lineOfSight(0, phase))
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
		if (tileX == play.getX() && lineOfSight(1, phase))
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
		int nextX = static_cast<int>(tileX + moveX);			// Calculate next tile position based on input to avoid 
		int nextY = static_cast<int>(tileY + moveY);			// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.

		if (moveX != 0)						// If moving horizontally,
			if (isObstructed(nextX, tileY, phase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(phase);			//recovery variables say that when colliding into a wall recenter in the pod
				float recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
				moveX = recoverX / speed;
				float recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
				moveY = recoverY / speed;
			}

		if (moveY != 0)						// If moving vertically,
			if (isObstructed(tileX, nextY, phase))		// If pod in next tile is solid and colliding, stop + change direction
			{
				changeDirection(phase);			//recovery variables say that when colliding into a wall recenter in the pod
				float recoverY = (tileY * _scaledTile + _halfScaled) - getSprite().getPosition().y;
				moveY = recoverY / speed;
				float recoverX = (tileX * _scaledTile + _halfScaled) - getSprite().getPosition().x;
				moveX = recoverX / speed;
			}

		move(moveX * speed, moveY * speed);
	}
}

void Enemy::changeDirection(bool phase)
{
	dir = static_cast<Entity::Facing>(rand() % 4);

	switch (dir)
	{
	case Facing::Up:

		moveX = 0;
		moveY = -1;

		if (!phase&&!pods[tileY - 1][tileX].isFilled)
			break;
		if (phase && !pods[tileY - 1][tileX].isHard)
			break;

		[[fallthrough]];

	case Facing::Down:

		moveX = 0;
		moveY = 1;

		if (!phase && !pods[tileY + 1][tileX].isFilled)
			break;
		if (phase && !pods[tileY + 1][tileX].isHard)
			break;

		[[fallthrough]];

	case Facing::Left:

		moveX = -1;
		moveY = 0;

		if (!phase && !pods[tileY][tileX - 1].isFilled)
			break;
		if (phase && !pods[tileY][tileX - 1].isHard)
			break;

		[[fallthrough]];

	case Facing::Right:

		moveX = 1;
		moveY = 0;

		if (!phase && !pods[tileY][tileX + 1].isFilled)
			break;
		if (phase && !pods[tileY][tileX + 1].isHard)
			break;

		[[fallthrough]];

	default:

		moveX = 0;
		moveY = 0;
	}
}

bool Enemy::lineOfSight(bool xy, bool phase)	//xy-0means x direction, 1 means y
{
	bool blocked = false;
	if (xy == 0)
	{
		for (int i = 1; i < (tileX - play.getX()); i++)
		{
			if (sightObstruction(tileX - i, tileY, phase))
				blocked = true;
		}
		for (int i = 1; i < (play.getX() - tileX); i++)
		{
			if (sightObstruction(tileX + i, tileY, phase))
				blocked = true;
		}
	}
	else
	{
		for (int i = 1; i < (tileY - play.getY()); i++)
		{
			if (sightObstruction(tileX, tileY - i, phase))
				blocked = true;
		}
		for (int i = 1; i < (play.getY() - tileY); i++)
		{
			if (sightObstruction(tileX, tileY + i, phase))
				blocked = true;
		}
	}
	return !blocked;
}

bool Enemy::isObstructed(int checkX, int checkY, bool phase)
{
	if (checkX < 0 || checkX >= _cols ||		// Out of bounds, treat as solid
		checkY < 0 || checkY >= _rows)
		return true;

	// return if pod in question is solid and colliding
	if (!phase)
		return pods[checkY][checkX].isFilled && intersects(checkX, checkY);
	return pods[checkY][checkX].isHard && intersects(checkX, checkY);
}

bool Enemy::sightObstruction(int checkX, int checkY, bool phase)
{
	if (checkX < 0 || checkX >= _cols ||		// Out of bounds, treat as solid
		checkY < 0 || checkY >= _rows)
		return true;

	// return if pod in question is solid and colliding
	if (!phase)
		return pods[checkY][checkX].isFilled;
	return pods[checkY][checkX].isHard;
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

	/*out << "state: ";

	switch (enemy.state)
	{
	case Entity::State::Living:	out << "living";	break;
	case Entity::State::Dead:	out << "dead";		break;
	case Entity::State::Dying:	out << "dying";		break;
	case Entity::State::Exit:	out << "exit";		break;
	}*/

	out << "\n";

	return out;
}

Enemy& Enemy::operator=(const Enemy& other)
{
	if (this != &other)
	{
		Entity::operator=(other);

		type = other.type;
		lastFacing = other.lastFacing;

		moveX = other.moveX;
		moveY = other.moveY;

		speed = other.speed;
		eratic = other.eratic;

		enemyFrameYPos = other.enemyFrameYPos;
	}
	return *this;
}