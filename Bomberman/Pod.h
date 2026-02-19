#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Pod
{
public:
	Pod(sf::RectangleShape, int, int);
	Pod() : tile(nullptr) {}
	~Pod();

	// Sets the object occupying the pod.
	void setTile(Tile*);

	// Returns the object occupying the pod.
	Tile* getTile();

	// Removes the tile from the pod.
	void deleteTile();

	// Returns true if the pod is impassable.
	bool isObstructed();

	void setColor(sf::Color);//Used to test bomb


	// ***** TYSON START *****

	
	void fill();

	sf::RectangleShape shape;
	bool filled;
	int x, y;

	// ***** TYSON END *****

private:
	// The object occupying the pod. Null by default.
	Tile* tile;
};