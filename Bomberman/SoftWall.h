#pragma once
#include "Tile.h"

class SoftWall : public Tile
{
public:
	SoftWall() : Tile(SOFT_WALL) {}
	~SoftWall() {}
	bool isObstruction() const { return true; }
	bool isDestructible() const { return true; }
};