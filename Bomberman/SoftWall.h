#pragma once
#include "Entity.h"
#include "Constants.h"

class Pod;

/*
	SoftWall class, represents
	the destructable walls in game,
	and break when hit by an explosion.
*/

class SoftWall : public Entity
{
public:
	SoftWall(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], int, int);

	void update();
	void animate();
	void die();

	SoftWall& operator=(const SoftWall&);
};