#pragma once
#include "Tile.h"

class Pod
{
public:
	Pod();
	~Pod();

	bool isObstructed();

private:
	//The object occupying the pod
	Tile* tile;

	void setTile(Tile*);
	Tile* getTile();
};