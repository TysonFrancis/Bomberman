#pragma once
#include "Tile.h"

class Pod
{
public:
	Pod();
	~Pod();

	bool isObstructed();
	void setTile(int);
	Tile* getTile();
	void deleteTile();

private:
	//The object occupying the pod
	Tile* tile;
};