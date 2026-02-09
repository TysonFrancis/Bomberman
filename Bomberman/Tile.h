#pragma once

/*
	Not to be confused with Pod, this class represents a wall, bomb, or item
	contained within a Pod
	TODO: actually impliment
*/

class Tile
{
public:
	Tile(int);
	~Tile() {}

	bool isObstruction() const;

	enum Type
	{
		HARD_WALL,
		SOFT_WALL,
		BOMB
		//ETC
	};

private:
	//Whether the movement/explosions should be blocked by this tile
	bool obstruction;
};