#include "Enemy.h"
#include <iostream>

using namespace Constants;

using sf::Keyboard::isKeyPressed;
using sf::Keyboard::Scancode;

Enemy::Enemy(const sf::Texture& tex, Pod (&pod)[_rows][_cols], Type input) :
	Entity(tex), pods(pod), type(input), speed(0.f)
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
	
	// Instead of case logic, can use enum values to calculate
	// texture position, since textures are in order of enum.
	// Same with initial death frames before they split into 3 colors.
	setTexture(sf::IntRect({ 0, 240 + static_cast<int>(type) * _tileSize}, {_tileSize, _tileSize}));
	direct = rand() % 4;
}

void Enemy::update()
{
	// Movement
	if (alive)
	{
		int x = getSprite().getPosition().x / _scaledTile;
		int y = getSprite().getPosition().y / _scaledTile;
		bool pause = (rand() % 9 == 0);
		if ((int)(sprite.getPosition().x) % _scaledTile == 0 || (int)(getSprite().getPosition().y) % _scaledTile == 0)
			pause = false;
		int moveX = 0;
		int moveY = 0;
		std::cout << "(" << x << "," << y<<","<<direct<< ")\n";
		switch (type)
		{
		case Type::Ballom: //ballom, random movement
			if (!pause)
			{
				switch (direct)
				{								//Current issue: Never collides with the pod. It knows which pod is which, which pod to check, and where
				case 0:							//it is it simply never has collide call true. Not an issue with the intersects method.
					moveX = speed;
					//std::cout << "case0";
					if (pods[y][x + 1].getTile() != nullptr)
					{
						//std::cout << "case00";
						if (intersects(pods[y][x + 1]))
						{
							std::cout << "case000";
							if (pods[y][x + 1].getTile()->isObstruction())
							{
								moveX = 0;
								direct = rand() % 4;
								//std::cout << "hit0";
							}
						}
					}
					move(sf::Vector2f(moveX, moveY));
					break;
				case 1:
					moveY = -speed;
					//std::cout << "case1";
					if (pods[y - 1][x].getTile() != nullptr)
					{
						//std::cout << "case11";
						if (intersects(pods[y - 1][x]))
						{
							std::cout << "case111";
							if (pods[y - 1][x].getTile()->isObstruction())
							{
								moveY = 0;
								direct = rand() % 4;
								//std::cout << "hit1";
							}
						}
					}
					move(sf::Vector2f(moveX, moveY));
					break;
				case 2:
					moveX = -speed;
					//std::cout << "case2";
					if (pods[y][x - 1].getTile() != nullptr)
					{
						//std::cout << "case22";
						if (intersects(pods[y][x - 1]))
						{
							std::cout << "case222";
							if (pods[y][x - 1].getTile()->isObstruction())
							{
								moveX = 0;
								direct = rand() % 4;
								//std::cout << "hit2";
							}
						}
					}
					move(sf::Vector2f(moveX, moveY));
					break;
				case 3:
					moveY = speed;
					//std::cout << "case3";
					if (pods[y + 1][x].getTile() != nullptr)
					{
						//std::cout << "case33";
						if (intersects(pods[y + 1][x]))
						{
							std::cout << "case333";
							if (pods[y + 1][x].getTile()->isObstruction())
							{
								moveY = 0;
								direct = rand() % 4;
								//std::cout << "hit3";
							}
						}
					}
					move(sf::Vector2f(moveX, moveY));
					break;
				}
				
			}
			else
			{
				direct = rand() % 4;
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

	if(myTick % 5 != 0)						// Only update frame every 5 ticks, 60fps -> 12 frames per second
		return;

	if (alive)							    // If alive,
	{
		myFrame = (myFrame + 1) % 3;			// Loop through frames for walking animation, 3 frames total
		return;

		// Movement animations soon to come - Dylan
	}

	if (myTick < _fps)						// Don't continue with death animation until after 1 second
		return;

	if (myFrame < 4)						// Keep incrementing frame until finished with death animation
		myFrame++;

	switch (type)							// Death animations
	{
	case Type::Ballom:							// Pink deaths
	case Type::Minvo:
	case Type::Pass:
	case Type::Pontan:
		setTexture(sf::IntRect({ myFrame * 16 + 112, 240 }, { _tileSize, _tileSize }));
		break;

	case Type::Onil:							// Blue deaths
	case Type::Doria:
		setTexture(sf::IntRect({ myFrame * 16 + 112, 288 }, { _tileSize, _tileSize }));
		break;

	case Type::Dahl:							// Purple deaths
	case Type::Ovape:
		setTexture(sf::IntRect({ myFrame * 16 + 112, 272 }, { _tileSize, _tileSize }));
		break;
	}
}

void Enemy::die()
{
	alive = false;
	myFrame = myTick = 0;
	setTexture(sf::IntRect({ 96, 240 + static_cast<int>(type) * _tileSize }, { _tileSize, _tileSize }));
}

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