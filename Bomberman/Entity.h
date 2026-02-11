#pragma once
#include <SFML/Graphics.hpp>

#include "Animations.h"

using sf::Vector2f;

/*
	Entity class, parent to Player, Enemies, and Bombs, 
	anything that needs a texture and life state.
	Holds their sprite and life status as well as methods to access them.
	Accepts a texture to apply to sprite, given from an animation object.
*/

class Entity
{
public:
	Entity(sf::Texture&);

	sf::Sprite& getSprite();

	void setLife(bool);
	bool getLife();

	void move(Vector2f);
	void setScale(Vector2f);
	void setPosition(Vector2f);
	void setOrigin(Vector2f);

	void setTexture(const sf::IntRect&);

	bool intersects(Entity*);

protected:
	sf::Sprite sprite;
	bool alive;
	int myFrame; 
	int myTick;
};