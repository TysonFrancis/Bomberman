#pragma once
#include <SFML/Graphics.hpp>

#include "Tile.h"
#include "Pod.h"
#include "Constants.h"

class Bomb : public Tile
{
public:
	Bomb(bool, int);

	void tick();
	void explode();

	int getType() const { return BOMB; }
	int getTicks() const { return ticks; }

private:
	int distance;
	int ticks;
	int myFrame, myTick;
	bool placed;
};