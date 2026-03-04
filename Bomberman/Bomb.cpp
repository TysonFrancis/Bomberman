#include "Bomb.h"

Bomb::Bomb(const sf::Texture& tex, bool timer, int d, 
	Pod(&pod)[_rows][_cols],int inY, int inX) :
	Entity(tex), pods(pod), remote(timer), distance(d), y(inY), x(inX) {}

Bomb::~Bomb() {}


void Bomb::update()
{
	if (myTick >= _fps * 3 && !remote)	// If 3 seconds and no remote explode
		explode();

	animate();
}

void Bomb::explode()
{
	state = State::Dead;
	pods[y][x].deleteTile();	// Deletes spot it's at

	explodeDirection(1, 0);		// Checks right
	explodeDirection(-1, 0);	// Checks left
	explodeDirection(0, 1);		// Checks down
	explodeDirection(0, -1);	// Checks up
}


void Bomb::animate()
{
	myTick++;

	if (myTick % 18 != 0) //Changes size every 18 ticks
		return;
}

Bomb& Bomb::operator=(const Bomb& other)
{
	if (this != &other)
		Entity::operator=(other);
	return *this;
}

void Bomb::explodeDirection(int xDir, int yDir)
{
	for (int i = 1; i <= distance; i++)
	{
		int xPos = x + xDir * i, yPos = y + yDir * i;		// Resized postion to check based on direction and distance
		Tile* tile = pods[yPos][xPos].getTile();			// Get tile at position

		if (pods[yPos][xPos].getTile() == nullptr)			// Skip to next check if empty tile
			continue;		// This skips to next loop iteration
		
		if (pods[yPos][xPos].getTile()->getType() == Tile::Type::SOFT_WALL)		// If a soft wall is in the way,
		{																		// delete it and stop checking in that direction
			pods[yPos][xPos].deleteTile();
			break;											// Important to break, else could explode through walls 
		}
	}
}