#include "Pod.h"

Pod::Pod() {};

Pod::~Pod() {};

bool Pod::isObstructed()
{
	if (tile == nullptr)
		return false;
	else
		return tile->isObstruction();
}

void Pod::setTile(int newType) //Makes a new tile in this pod
{
	new Tile(newType);
	tile = new Tile(newType); //WILL NEED TO DEALLOCATE MEMORY LATER
}

Tile* Pod::getTile()
{
	return tile;
}

void Pod:: deleteTile()
{
	delete tile;
	tile = nullptr;
}

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