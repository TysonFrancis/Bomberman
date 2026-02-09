#pragma once
#include <SFML/Graphics.hpp>

#include "Animations.h"

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

protected:
	sf::Sprite sprite;
	bool alive;
	int myFrame;
	int myTick;
};