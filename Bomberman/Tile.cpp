#include "Tile.h"

Tile::Tile(int type) : obstruction(false)
{
	switch (type)
	{
		case HARD_WALL:
			break;
		case SOFT_WALL:
			break;
		case BOMB:
			obstruction = true;
			break;
	}
}

bool Tile::isObstruction() const { return obstruction; }