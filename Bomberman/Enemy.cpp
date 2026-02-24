#include "Enemy.h"

using ET = EnemyType;

Enemy::Enemy(const sf::Texture& tex, EnemyType input) :
	Entity(tex), type(input)
{
	switch (type)
	{
	case ET::Ballom: speed = 2.f; break;
	case ET::Onil:	 speed = 3.f; break;
	case ET::Dahl:	 speed = 3.f; break;
	case ET::Minvo:	 speed = 4.f; break;
	case ET::Doria:	 speed = 1.f; break;
	case ET::Ovape:	 speed = 2.f; break;
	case ET::Pass:	 speed = 5.f; break;
	case ET::Pontan: speed = 6.f; break;
	}
}

void Enemy::update()
{
	switch (type)
	{
	case ET::Ballom: //ballom, random movement
		break;

	case ET::Onil: //onil, chases player if close
		break;

	case ET::Dahl: //dahl, random movement
		break;

	case ET::Minvo: //minvo, chases player
		break;

	case ET::Doria: //doria, chases, avoids bombs, moves through soft blocks
		break;

	case ET::Ovape: //ovape, random movement, moves through soft blocks
		break;

	case ET::Pass: //pass, always chases if encountered
		break;

	case ET::Pontan: //pontan, alwyays chases, moves through soft blocks
		break;
	}
}

void Enemy::die() {}