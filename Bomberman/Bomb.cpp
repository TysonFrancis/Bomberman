#include "Bomb.h"

Bomb::Bomb(int xPosition, int yPosition, bool timer, Pod pods[11][29], int d) :
	x(xPosition), y(yPosition), ticks(0), distance(d), Tile(BOMB)
{
	//timer is a bool that will show if the player has the remote control power up
	if (timer)
		tick();
}


Bomb::~Bomb() {}


void Bomb::tick()
{
	ticks++; //3 seconds
}


void Bomb::explode()
{

	/*//Check if the squares in each direction will stop explosion
	//right
	for (int i = 0; i < distance; i++)
	{
		if (x + i < 29) //Make sure not out of bounds
		{
			if ((pods[y][x + i].getTile())->isObstruction())//Checks if wall
			{
				i = distance; //stops explosion from going further
				//Check if soft
			}
			else pods[y][x + i].setColor(sf::Color(255, 0, 0));
		}
	}*/
}