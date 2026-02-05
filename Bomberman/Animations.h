#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Animations
{
public:
	void update(float dt);
	void draw(sf::RenderWindow& window);

	void setPosition(float x, float y);
	void setScale(float x, float y);

private:
	sf::Texture texture;
	sf::Sprite sprite;
};