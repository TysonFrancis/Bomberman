#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>

class Pod
{
/*public:
	Pod();
	~Pod();

	bool isObstructed();

private:
	//The object occupying the pod
	Tile* tile;

	void setTile(Tile*);
	Tile* getTile();*/

	// ********** TYSON LOGIC STUFF ********** START ********** //
public:
	Pod(sf::RectangleShape, int, int);
	void fill();

	sf::RectangleShape shape;
	bool filled;
	int x, y;
	// ********** END ********** //
};