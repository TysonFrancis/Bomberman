#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(sf::Texture&, int);

	void move();

	void update();
	void die();

	bool isAlive();

	sf::FloatRect getBounds() const;

private:
	int type;
	float speed;
};