#include "Points.h"
#include <iostream>

using namespace Constants;

Points::Points(const sf::Texture& tex, Pod(&pods)[_rows][_cols], int input,int inX, int inY):
Entity(tex,pods),value(input), x(inX), y(inY)
{
	setPosition(x, y);
	if (value <= 1000)
		sprite.setTextureRect(sf::IntRect(sf::Vector2i(116, 8 * static_cast<int>(log2(value / 100)) + 338), _pointsTile));
	else
		sprite.setTextureRect(sf::IntRect(sf::Vector2i(116, 8 * static_cast<int>(log2(value / 1000)) + 338), _pointsTile));
}


void Points::update()
{
	tick++;
	if (tick >= 240)//Die after 2 seconds
		state = State::Dead;
}


Points& Points::operator=(const Points& other)
{
	if (this != &other)
	{
		Entity::operator=(other);

		value = other.value;
	}
	return *this;
}