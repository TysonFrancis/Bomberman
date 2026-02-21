#include "Enemy.h"

Enemy::Enemy(const sf::Texture& tex, int input) :
	Entity(tex), type(input), speed(4.f) {}

void Enemy::move()
{
	switch (type)
	{
	case 1: //ballom, random movement, speed 2
		break;

	case 2: //onil, chases player if close, speed 3
		break;

	case 3: //dahl, random movement,speed 3
		break;

	case 4: //minvo, chases player, speed 4
		break;

	case 5: //doria, chases, avoids bombs, speed 1, moves through soft blocks
		break;

	case 6: //ovape, random movement, speed 2, moves through soft blocks
		break;

	case 7: //pass, always chases if encountered, speed 5
		break;

	case 8: //pontan, alwyays chases, speed 6, moves through soft blocks
		break;
	}
}

void Enemy::update() {}
void Enemy::die() {}