#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Animations
{
public:
	Animations();

	void draw(sf::RenderWindow& window);
	void setPosition(float x, float y);
	void setScale(float x, float y);

	sf::Texture getEntities();
	sf::Texture getBackground();
	sf::Texture getMisc();
	sf::Texture getTitle();

private:
	sf::Texture entities;
	sf::Texture background;
	sf::Texture misc;
	sf::Texture title;
};