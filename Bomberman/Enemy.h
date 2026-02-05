#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	void move();

private:
	int type;
};