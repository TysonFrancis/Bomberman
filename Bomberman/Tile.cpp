#include "Tile.h"

Tile::Tile(int type) : obstruction(false)
{
	switch (type)
	{
	case HARD_WALL:
	case SOFT_WALL:
	case BOMB:
		obstruction = true;
		break;
	default:
		// unreachable but compiler won't stfu about uninitialized variables
		// if anyone knows how to suppress that please do so we can delete this
		obstruction = false;
	}
}

bool Tile::isObstruction() const { return obstruction; }