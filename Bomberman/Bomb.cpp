#include "Bomb.h"

Bomb::Bomb(float xPosition, float yPosition, bool timer, int pods[10][10], int d) :
	xPosition(xPosition), yPosition(yPosition), ticks(0), distance(d)//, Tile(BOMB)
{
	//timer is a bool that will show if the player has the remote control power up
	if (timer)
		tick(pods);
}


Bomb::~Bomb() {}


void Bomb::tick(int pods [10][10])
{
	ticks++;
	if (ticks >= 180) //3 seconds
		explode(pods);
}


void Bomb::explode(int pods [10][10])
{
	//Check if the squares in each direction will stop explosion
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
	} while (pods[xPosition][yPosition--] != 1 && distance > 0);
}