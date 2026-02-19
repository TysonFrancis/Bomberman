#pragma once
#include "Tile.h"

class HardWall : public Tile
{
public:
	HardWall() : Tile(HARD_WALL) {}
	~HardWall() {}
	bool isObstruction() const { return true; }
	bool isDestructible() const { return false; }
};