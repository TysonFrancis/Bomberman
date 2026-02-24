#pragma once
#include <SFML/Graphics.hpp>

#include "Animations.h"
#include "Pod.h"

using sf::Vector2f;

/*
	Entity class, parent to Player, Enemies, and Bombs, 
	anything that needs a texture and life state.
	Holds their sprite and life status as well as methods to access them.
	Accepts a texture to apply to sprite, given from an animation object.
*/

class Entity : public sf::Drawable
{
public:
	Entity(const sf::Texture&);

	const sf::Sprite& getSprite() const;
	bool getLife() const;

	void setLife(bool);

	void move(Vector2f);
	void setScale(Vector2f);
	void setPosition(Vector2f);
	void setOrigin(Vector2f);
	void setTexture(const sf::IntRect&);

	bool intersects(Entity&) const;
	bool intersects(Pod&) const;

	// So game can just draw with entitiy instead of entity.sprite()
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

protected:
	sf::Sprite sprite;
	bool alive;
	int myFrame; 
	int myTick;
};