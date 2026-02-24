#pragma once

/*
	Not to be confused with Pod, this abstract class
	parents a wall, bomb, or item contained within a Pod
*/

class Tile
{
public:
	// Pass the type for easy checking later
	Tile(int);

	~Tile() {}

	int getType() const;

	// True if this tile type should block movement
	bool isObstruction() const;
	bool isDestructible() const;
	
	void tick() { ticks++; }
	int getTicks() const { return ticks; }

	enum Type
	{
		HARD_WALL,
		SOFT_WALL,
		BOMB
	};
	
private:
	// Whether the movement/explosions should be blocked by this tile
	bool obstruction;

	// Identifies the child class
	int type; 
	int ticks;
};