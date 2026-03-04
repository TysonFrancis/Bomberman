#pragma once
#include <SFML/Graphics.hpp>

#include "Animations.h"
#include "Pod.h"

/*
	Entity class, parent to Player, Enemies, and Bombs, 
	anything that needs a texture and life state.
	Holds their sprite and life status as well as methods to access them.
	Accepts a texture to apply to sprite, given from an animation object.
*/

class Entity : public sf::Drawable
{
public:
	enum class State { Living, Dying, Dead };
	enum class Facing { Up, Down, Left, Right };

	Entity(const sf::Texture&);

	const sf::Sprite& getSprite() const;
	State getState() const;

	void move(sf::Vector2f);
	void setScale(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void setOrigin(sf::Vector2f);
	void setTexture(const sf::IntRect&);

	bool intersects(Entity&) const;
	bool intersects(Pod&) const;

	// So game can just draw with entitiy instead of entity.sprite()
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	Entity& operator=(const Entity& other);

protected:
	sf::Sprite sprite;
	State state;
	Facing dir;
	int myFrame; 
	int myTick;
};