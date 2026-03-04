#include "Enemy.h"
#include <iostream>

using namespace Constants;

using sf::Keyboard::isKeyPressed;
using sf::Keyboard::Scancode;

Enemy::Enemy(const sf::Texture& tex, Pod (&pod)[_rows][_cols], Type input) :
	Entity(tex), pods(pod), type(input), speed(0.f), lastFacing(Facing::Left)
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
	setTexture(sf::IntRect({ 0, 240 + static_cast<int>(type) * _tileSize}, {_tileSize, _tileSize}));
	dir = randomDirection();
}

void Enemy::update()
{
	// Movement
	if (state == State::Living)
	{
		int x = (getSprite().getPosition().x +24)/ _scaledTile;
		int y = (getSprite().getPosition().y+24) / _scaledTile;
		int tileX = x*_scaledTile;
		int tileY = y * _scaledTile;
		bool pause = (rand() % 9 == 0);
		if (tileX - getSprite().getPosition().x != 0 || tileY - getSprite().getPosition().y != 0)
			pause = false;
		if ((int)(sprite.getPosition().x) % _scaledTile == 0 || (int)(getSprite().getPosition().y) % _scaledTile == 0)
			pause = false;
		int moveX = 0;
		int moveY = 0;
		switch (type)
		{
		case Type::Ballom: //ballom, random movement
			if (!pause)
			{
				switch (dir)
				{
				case Facing::Right:
					moveX = speed;
					//std::cout << "case0";
					if (pods[y][x + 1].getTile() != nullptr)
					{
						//std::cout << "case00";
						if (intersects(pods[y][x+1]))
						{
							//std::cout << "case000";
							if (pods[y][x + 1].getTile()->isObstruction())
							{
								moveX = 0;
								dir = randomDirection();
								//std::cout << "hit0";
							}
						}
					}
					break;
				case Facing::Up:
					moveY = -speed;
					//std::cout << "case1";
					if (pods[y - 1][x].getTile() != nullptr)
					{
						//std::cout << "case11";
						if (intersects(pods[y - 1][x]))
						{
							//std::cout << "case111";
							if (pods[y - 1][x].getTile()->isObstruction())
							{
								moveY = 0;
								dir = randomDirection();
								//std::cout << "hit1";
							}
						}
					}
					break;
				case Facing::Left:
					moveX = -speed;
					//std::cout << "case2";
					if (pods[y][x - 1].getTile() != nullptr)
					{
						//std::cout << "case22";
						if (intersects(pods[y][x - 1]))
						{
							//std::cout << "case222";
							if (pods[y][x - 1].getTile()->isObstruction())
							{
								moveX = 0;
								dir = randomDirection();
								//std::cout << "hit2";
							}
						}
					}
					break;
				case Facing::Down:
					moveY = speed;
					//std::cout << "case3";
					if (pods[y + 1][x].getTile() != nullptr)
					{
						//std::cout << "case33";
						if (intersects(pods[y + 1][x]))
						{
							//std::cout << "case333";
							if (pods[y + 1][x].getTile()->isObstruction())
							{
								moveY = 0;
								dir = randomDirection();
								//std::cout << "hit3";
							}
						}
					}
					break;
				}
				move(sf::Vector2f(moveX, moveY));
			}
			else
			{
				dir = randomDirection();
			}
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

		if (isKeyPressed(Scancode::Y))
			die();
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
		myFrame = (myFrame + 1) % 3;			// Loop through frames for walking animation, 3 frames total

		if(facing == Facing::Left)
			setTexture(sf::IntRect({ myFrame * 16 + 48, 240 }, { _tileSize, _tileSize }));
		else
			setTexture(sf::IntRect({ myFrame * 16, 240 }, { _tileSize, _tileSize }));
		

		return;
	}

	if (myTick < _fps)						// Don't continue with death animation until after 1 second
		return;

	if (myFrame < 4)						// Keep incrementing frame until finished with death animation
		myFrame++;

	if (myFrame >= 4)						// Once animation is finished, fully diezzz
		state = State::Dead;

	int deathRow = 240;							// Deafault to pink death row
	if(type == Type::Onil || type == Type::Doria)
		deathRow = 288;							// Set to blue death row
	else if (type == Type::Dahl || type == Type::Ovape)
		deathRow = 272;							// Set to purple death row

	setTexture(sf::IntRect({ myFrame * 16 + 112, deathRow }, { _tileSize, _tileSize }));
}

void Enemy::die()
{
	state = State::Dying;
	myFrame = myTick = 0;
	setTexture(sf::IntRect({ 96, 240 + static_cast<int>(type) * _tileSize }, { _tileSize, _tileSize }));
}

Enemy& Enemy::operator=(const Enemy& other)
{
	if (this != &other)
		Entity::operator=(other);
	return *this;
}

Entity::Facing Enemy::randomDirection() { return static_cast<Entity::Facing>(rand() % 4); }

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