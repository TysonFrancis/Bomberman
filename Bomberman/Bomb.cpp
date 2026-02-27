#include "Bomb.h"

Bomb::Bomb(const sf::Texture& tex, bool timer, int d, 
	Pod(&pod)[_rows][_cols],int inY, int inX) :
	Entity(tex), pods(pod), remote(timer), distance(d), y(inY), x(inX) {}

Bomb::~Bomb() {}


void Bomb::update()
{
	myTick++;
	if(myTick>=60 && alive)
		explode();
}

void Bomb::explode()
{
	alive = false;
	pods[y][x].deleteTile();
	pods[y][x].setColor(sf::Color::Yellow); //temporary color change to test bomb explosion
	for (int i = 1; i <= distance; i++)
	{

		if (pods[y][x + i].getTile() == nullptr)
			pods[y][x + i].setColor(sf::Color::Yellow);//temporary color change to test bomb explosion
		else
		{
		if (pods[y][x + i].getTile()->getType() == 1)
			pods[y][x + i].deleteTile();
		break;
		}


	};
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