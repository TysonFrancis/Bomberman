#include "Points.h"
#include <iostream>

using namespace Constants;

Points::Points(const sf::Texture& tex, Pod(&pods)[_rows][_cols], int input,int inX, int inY):
Entity(tex,pods),value(input), x(inX), y(inY)
{
	sprite.setPosition(sf::Vector2f(x + (_tileSize / 2), y + (_tileSize / 2)));
	if (value <= 1000)
		sprite.setTextureRect(sf::IntRect(sf::Vector2i(116, 8 * log2(value / 100) + 338), sf::Vector2i(16, 6)));
	else
		sprite.setTextureRect(sf::IntRect(sf::Vector2i(116, 8 * log2(value / 1000) + 338), sf::Vector2i(16, 6)));
}


void Points::update()
{
	tick++;
	if (tick >= 120)//Die after 2 seconds
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