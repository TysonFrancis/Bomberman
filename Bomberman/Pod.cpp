#include "Pod.h"

Pod::Pod() {}

Pod::~Pod() { delete tile; }

bool Pod::isObstructed()
{
	if (tile == nullptr)
		return false;
	else
		return tile->isObstruction();
}

// Makes a new tile in this pod
void Pod::setTile(Tile* newTile) { tile = newTile; }

Tile* Pod::getTile() { return tile; }

void Pod:: deleteTile()
{
	delete tile;
	tile = nullptr;
}


// TYSON STUFF BELOW THIS POINT

Pod::Pod(sf::RectangleShape shapes, int ex, int why)
{
	shape = shapes;
	filled = true;
	x = ex;
	y = why;

}

void Pod::fill()
{
	filled = true;
	shape.setFillColor(sf::Color(0, 125, 125));
}