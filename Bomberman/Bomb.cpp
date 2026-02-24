#include "Bomb.h"

Bomb::Bomb(bool timer, int d) :
	ticks(0), distance(d), Tile(BOMB)
{
	//timer is a bool that will show if the player has the remote control power up
	myFrame = myTick = 0; placed = true;
}

void Bomb::tick()
{
	// Incomplete I just don't have brain power to figure out what I want
	myTick++;									// Increment tick counter every game frame

	if (placed)								    // If alive,
	{
		if (myTick % 5 == 0)				        // Update frame every 5 ticks, 60fps -> 12 frames per second
			myFrame = (myFrame + 1) % 2;	        // Loop through frames for waling animation, 3 frames total
	}

	else                                        // Else,
	{
		if (myTick % 5 == 0)
			myFrame++;								// Update frame every 5 ticks, but don't loop
		if (myFrame >= 7)
			myFrame = 7;                            // Freeze on empty frame once finished
	}
}

void Bomb::explode()
{
	placed = false;
	myFrame = myTick = 0;
}