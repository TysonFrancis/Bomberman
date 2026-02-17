#include "Bomb.h"

Bomb::Bomb(int xPosition, int yPosition, bool timer, Pod pods[11][29], int d) :
	xPosition(xPosition), yPosition(yPosition), ticks(0), distance(d), Tile(BOMB)
{
	//timer is a bool that will show if the player has the remote control power up
	if (timer)
		tick();
}


Bomb::~Bomb() {}


void Bomb::tick()
{
	ticks++;
	if (ticks >= 180) //3 seconds
		explode();
}


void Bomb::explode()
{
	/*Check if the squares in each direction will stop explosion
	do
	{
		pods[xPosition++][yPosition] = 0; //Pod at this location becomes dangerous
		distance--;
		//while statement checks if next location has a wall or if distance is 0
	} while (pods[xPosition++][yPosition] != 1 && distance > 0);

	//repeat for each direction
	do
	{
		pods[xPosition--][yPosition] = 0;
		distance--;
	} while (pods[xPosition--][yPosition] != 1 && distance > 0);

	do
	{
		pods[xPosition][yPosition++] = 0;
		distance--;
	} while (pods[xPosition][yPosition++] != 1 && distance > 0);

	do
	{
		pods[xPosition][yPosition--] = 0;
		distance--;
	} while (pods[xPosition][yPosition--] != 1 && distance > 0);*/
}