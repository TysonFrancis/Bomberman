#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Bomb : public Entity
{
public:
	Bomb(float, float, bool, int[10][10]);
	~Bomb();
	void explode(int[10][10]);

private:
	int distance;
	int xPosition;
	int yPosition;
};