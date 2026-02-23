#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Pod.h"
#include "Constants.h"

class Bomb : public Tile
{
public:
	Bomb(bool, int); 
	~Bomb();
	void tick();
	int getType() const { return BOMB; }
	int getTicks() { return ticks; }
private:
	int distance;
	int ticks;
};