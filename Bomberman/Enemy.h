#pragma once
#include "Entity.h"

enum class EnemyType
{
	Ballom,
	Onil,
	Dahl,
	Minvo,
	Doria,
	Ovape,
	Pass,
	Pontan
};

class Enemy : public Entity
{
public:
	Enemy(const sf::Texture&, EnemyType);

	void update();
	void die();

private:
	EnemyType type;
	float speed;
};