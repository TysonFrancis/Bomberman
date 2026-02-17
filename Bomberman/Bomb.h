#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Pod.h"

class Bomb : public Tile
{
public:
	Bomb(int, int, bool, Pod [11][29], int); //Temporary array
	~Bomb();
	void tick();
	void explode();

private:
	int ticks;
	int distance;
	int xPosition;
	int yPosition;
};