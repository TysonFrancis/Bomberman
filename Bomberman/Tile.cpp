#include "Tile.h"

Tile::Tile(int type)
{
	switch (type)
	{
	case HARD_WALL:
	case SOFT_WALL:
	case BOMB:
		obstruction = true;
	}
}

Tile::~Tile() {}


bool Tile::isObstruction() const
{
	return obstruction;
}