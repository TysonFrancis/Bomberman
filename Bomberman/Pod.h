#pragma once
#include "Tile.h"

class Pod
{
public:
	Pod();
	~Pod();

private:
	//The object occupying the pod
	Tile* tile;

	//Whether the movement/explosions should be blocked by this pod
	bool obstruction;


	void setTile(Tile*);
	Tile* getTile();
};