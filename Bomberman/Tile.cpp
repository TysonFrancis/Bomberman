#include "Tile.h"

Tile::Tile(int type) : obstruction(false)
{
	switch (type)
	{
		case HARD_WALL:
		case SOFT_WALL:
		case BOMB:
			obstruction = true;
		case BOMB:
			obstruction = true;
			break;
		case BOMB:
			obstruction = true;
			break;
		case BOMB:
			obstruction = true;
			break;
		case BOMB:
			obstruction = true;
			break;
		case BOMB:
			obstruction = true;
			break;
	}
}

bool Tile::isObstruction() const { return obstruction; }