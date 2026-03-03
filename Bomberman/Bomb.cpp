#include "Bomb.h"

Bomb::Bomb(const sf::Texture& tex, bool timer, int d, 
	Pod(&pod)[_rows][_cols],int inY, int inX) :
	Entity(tex), pods(pod), remote(timer), distance(d), y(inY), x(inX) {}

Bomb::~Bomb() {}


void Bomb::update()
{
	animate();
	if(myTick>=180 && !remote)//If 3 seconds and no remote explode
		explode();
}

void Bomb::explode()
{
	alive = false;
	pods[y][x].deleteTile();//Deletes spot it's at
	for (int i = 1; i <= distance; i++) //Checks each direction
	{

		if (pods[y][x + i].getTile() == nullptr)
			pods[y][x + i].setColor(sf::Color::Yellow);//temporary color change to test bomb explosion
		else
		{
		if (pods[y][x + i].getTile()->getType() == 1)//Destroys soft
			pods[y][x + i].deleteTile();
		break;
		}
	}
	for (int i = 1; i <= distance; i++)
	{

		if (pods[y][x - i].getTile() == nullptr)
			pods[y][x - i].setColor(sf::Color::Yellow);
		else
		{
			if (pods[y][x - i].getTile()->getType() == 1)
				pods[y][x - i].deleteTile();
			break;
		}
	}
	for (int i = 1; i <= distance; i++)
	{
		if (pods[y + i][x].getTile() == nullptr)
			pods[y + i][x].setColor(sf::Color::Yellow);
		else
		{
			if (pods[y+i][x].getTile()->getType() == 1)
				pods[y+i][x].deleteTile();
			break;
		}
	};
	for (int i = 1; i <= distance; i++)
	{
		if (pods[y - i][x].getTile() == nullptr)
			pods[y - i][x].setColor(sf::Color::Yellow);
		else
		{
			if (pods[y - i][x].getTile()->getType() == 1)
			{
				pods[y - i][x].deleteTile();
			}
			break;
		}
	}
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
