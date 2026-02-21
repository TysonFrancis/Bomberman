#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(const sf::Texture&, int);

	void move();
	void update();
	void die();

private:
	int type;
	float speed;
};