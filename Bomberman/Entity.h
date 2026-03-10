#pragma once
#include <SFML/Graphics.hpp>

#include "Pod.h"
#include "Constants.h"

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
	enum class Facing { Up, Down, Left, Right, None };

	Entity(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols]);

	const sf::Sprite& getSprite() const;
	State getState() const;

	void move(sf::Vector2f);
	void setScale(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void setOrigin(sf::Vector2f);
	void setTexture(const sf::IntRect&);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool intersects(Entity&) const;
	bool intersects(float, float) const;

	Entity& operator=(const Entity& other);

protected:
	sf::Sprite sprite;

	Pod(&pods)[Constants::_rows][Constants::_cols];

	State state;
	Facing dir;

	int myTick;
	int myFrame;

	int tileX, tileY;
	int worldX, worldY;
};