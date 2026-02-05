#include "Pod.h"

Pod::Pod()
{
	tile = nullptr;
	obstruction = false;
}

Pod::~Pod() {}


void Pod::setTile(Tile* newTile)
{
	tile = newTile;
}


Tile* Pod::getTile()
{
	return tile;
}