#pragma once
#include <SFML/Graphics.hpp>

#include "Animations.h"
#include "Tile.h"

class Pod
{
public:
	Pod();
	Pod(const sf::Texture&, sf::RectangleShape, int, int);
	~Pod() { delete tile; }

	void update();

	// Sets the object occupying the pod.
	void setTile(Tile*);

	// Returns the object occupying the pod.
	Tile* getTile() const;

	const sf::RectangleShape& getShape() const;
	bool getFilled() const;
	int getX() const;
	int getY() const;

	// Removes the tile from the pod.
	void deleteTile();

	// Returns true if the pod is impassable.
	bool isObstructed();

	void setColor(sf::Color);//Used to test bomb
	void setTexture(const sf::IntRect&);

private:
	// The object occupying the pod. Null by default.
	Tile* tile;
	sf::RectangleShape shape;
	bool filled;
	int x, y;
};