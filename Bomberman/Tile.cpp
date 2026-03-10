//#include "Tile.h"
//
//Tile::Tile(int type) : type(type), obstruction(false) 
//{
//	switch (type)
//	{
//	case HARD_WALL:
//	case SOFT_WALL:
//	case BOMB:
//		// Because the above tiles block movement
//		obstruction = true;
//		// No need to specify tiles that don't because that's the default
//	}
//}
//
//int Tile::getType() const { return type; }
//
//bool Tile::isObstruction() const { return obstruction; }