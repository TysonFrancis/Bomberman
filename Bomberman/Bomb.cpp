#include "Bomb.h"

Bomb::Bomb( bool timer,  int d) :
 ticks(0), distance(d), Tile(BOMB)
{
	//timer is a bool that will show if the player has the remote control power up
	ticks=0;
}


Bomb::~Bomb() {}


void Bomb::tick()
{
	ticks++;
}