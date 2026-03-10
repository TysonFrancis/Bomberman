#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Pod.h"
#include "Constants.h"

class Explosion : public Entity
{
public:
	Explosion(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], int, int, Facing);

	void update();
	void animate();
	void propogate();

	Explosion& operator=(const Explosion& other);
};
