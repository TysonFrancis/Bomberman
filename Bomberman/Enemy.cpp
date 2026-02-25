#include "Enemy.h"

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
}

void Enemy::update()
{
	// *** Animation timing ***
	myTick++;									// Increment tick counter every game frame

	if (alive)								    // If alive,
	{
		if (myTick % 5 == 0)				        // Update frame every 5 ticks, 60fps -> 12 frames per second
			myFrame = (myFrame + 1) % 3;	        // Loop through frames for waling animation, 3 frames total
	}

	else                                        // Else,
	{
		if (myTick % 5 == 0)
			myFrame++;								// Update frame every 5 ticks, but don't loop
		if (myFrame >= 7)
			myFrame = 7;                            // Freeze on empty frame once finished
	}

	if (alive)
	{
		switch (type)
		{
		case Type::Ballom: //ballom, random movement
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
		{
			die();
			alive = false;
			myFrame = myTick = 0;
		}
	}

	else
	{
		switch (type)
		{
		case Type::Ballom: //ballom, random movement
			setTexture(sf::IntRect({ myFrame * 16 + 112, 240 }, { _tileSize, _tileSize }));
			break;

		case Type::Onil: //onil, chases player if close
			setTexture(sf::IntRect({ myFrame * 16 + 112, 288 }, { _tileSize, _tileSize }));
			break;

		case Type::Dahl: //dahl, random movement
			setTexture(sf::IntRect({ myFrame * 16 + 112, 272 }, { _tileSize, _tileSize }));
			break;

		case Type::Minvo: //minvo, chases player
			setTexture(sf::IntRect({ myFrame * 16 + 112, 240 }, { _tileSize, _tileSize }));
			break;

		case Type::Doria: //doria, chases, avoids bombs, moves through soft blocks
			setTexture(sf::IntRect({ myFrame * 16 + 112, 288 }, { _tileSize, _tileSize }));
			break;

		case Type::Ovape: //ovape, random movement, moves through soft blocks
			setTexture(sf::IntRect({ myFrame * 16 + 112, 240 }, { _tileSize, _tileSize }));
			break;

		case Type::Pass: //pass, always chases if encountered
			setTexture(sf::IntRect({ myFrame * 16 + 112, 240 }, { _tileSize, _tileSize }));
			break;

		case Type::Pontan: //pontan, alwyays chases, moves through soft blocks
			setTexture(sf::IntRect({ myFrame * 16 + 112, 240 }, { _tileSize, _tileSize }));
			break;
		}
	}								// If dead
}

void Enemy::die()
{
	switch (type)
	{
	case Type::Ballom:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 240 }, { _tileSize, _tileSize }));
		break;

	case Type::Onil:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 256 }, { _tileSize, _tileSize }));
		break;

	case Type::Dahl:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 272 }, { _tileSize, _tileSize }));
		break;

	case Type::Minvo:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 288 }, { _tileSize, _tileSize }));
		break;

	case Type::Doria:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 304 }, { _tileSize, _tileSize }));
		break;

	case Type::Ovape:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 320 }, { _tileSize, _tileSize }));
		break;

	case Type::Pass:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 336 }, { _tileSize, _tileSize }));
		break;

	case Type::Pontan:
		setTexture(sf::IntRect({ myFrame * 16 + 96, 352 }, { _tileSize, _tileSize }));
		break;
	}
}