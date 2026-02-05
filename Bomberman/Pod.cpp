#include "Pod.h"

Pod::Pod()
{
	tile = nullptr;
}

Pod::~Pod() {}


bool Pod::isObstructed()
{
	if (tile = nullptr)
		return false;
	else
		return tile->isObstruction();
}


void Pod::setTile(Tile* newTile)
{
	tile = newTile;
}


Tile* Pod::getTile()
{
	return tile;
}