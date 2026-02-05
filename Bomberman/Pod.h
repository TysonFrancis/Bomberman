#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


class Pod
{
public:
	Pod(sf::RectangleShape);
	~Pod();
	void fill();
	int filled;

	sf::RectangleShape shape;
};