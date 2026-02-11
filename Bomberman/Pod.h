#pragma once
#include "Tile.h"

class Pod
{
public:
	Pod();
	~Pod();

	// Sets the object occupying the pod.
	void setTile(Tile*);

	// Returns the object occupying the pod.
	Tile* getTile();

	// Returns true if the pod is impassable.
	bool isObstructed();
	void setTile(int);
	void deleteTile();

	Pod(sf::RectangleShape, int, int);
	void fill();

	sf::RectangleShape shape;
	bool filled;
	int x, y;

private:
	// The object occupying the pod. Null by default.
	Tile* tile;
};