#include "Bomb.h"

Bomb::Bomb(float xPosition, float yPosition, bool timer,
	int pods [10][10] )
{
	//timer is bool that will show if the player has the remote control power up
	if (timer)
	{
		sf::sleep(sf::seconds(3.0f));
		explode(pods);
	}
}


Bomb::~Bomb() {}


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