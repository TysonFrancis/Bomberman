#include "Pod.h"


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


void Pod::setColor(sf::Color color)
{
	shape.setFillColor(color);
}


// TYSON STUFF BELOW THIS POINT
Pod::Pod(sf::RectangleShape shapes, int x, int y)
{
	shape = shapes;
	filled = true;
	shape.setPosition(sf::Vector2f(x, y));
}

void Pod::fill()
{
	filled = true;
	shape.setFillColor(sf::Color(0, 125, 125));
}