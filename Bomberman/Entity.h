#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Pod;

/*
	Entity class, parent to Player, Enemy,
	Bomb, SoftWall, and Explosion, anything
	that needs applied textures and life states.
*/

class Entity : public sf::Drawable
{
public:
	enum class State { Living, Dying, Dead };
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

	bool isOnSameTile(const Entity&) const;
	bool intersects(const Entity&) const;
	bool intersects(int, int) const;

	Entity& operator=(const Entity&);

protected:
	sf::Sprite sprite;

	Pod(&pods)[Constants::_rows][Constants::_cols];

	State state;
	Facing dir;

	int tick;
	int frame;

	int tileX, tileY;
};