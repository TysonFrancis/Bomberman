#pragma once
#include <SFML/Graphics.hpp>

#include "Pod.h"
#include "Constants.h"

/*
	Entity class, parent to Player, Enemy, Bomb, SoftWall, and
	Explosion, anything that needs applied textures and life states.
	Holds their sprite, game pod system, position, animation items,
	and other information. Has getters for neccessary data members,
	as well as overridden SFML methods for code cleanliness.

	Constructor accepts a texture for the sprite, and a 2D pod array.

	Will eventually extend pure virtual methods to classes??
*/

class Entity : public sf::Drawable
{
public:
	enum class State { Living, Dying, Dead, Exit };
	enum class Facing { Up, Down, Left, Right, None };

	Entity(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols]);

	const sf::Sprite& getSprite() const;
	State getState() const;
	Facing getDir() const;
	int getX() const;
	int getY() const;

	void move(float, float);
	void setScale(float, float);
	void setPosition(int, int);
	void setOrigin(float, float);
	void setTexture(int, int);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;

	bool intersects(const Entity&) const;
	bool intersects(int, int) const;

	Entity& operator=(const Entity&);

protected:
	sf::Sprite sprite;

	Pod(&pods)[Constants::_rows][Constants::_cols];

	State state;
	Facing dir;

	int myTick;
	int myFrame;

	int tileX, tileY;
};