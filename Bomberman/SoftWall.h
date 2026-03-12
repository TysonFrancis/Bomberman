#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Pod.h"
#include "Constants.h"

class SoftWall : public Entity
{
public:
	SoftWall(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], int, int);

	void update();
	void animate();
	void die();

	SoftWall& operator=(const SoftWall&);
};