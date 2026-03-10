#pragma once
#include <SFML/Graphics.hpp>
#include "Pod.h"
#include "Constants.h"
#include "Entity.h"

class Bomb : public Entity
{
public:
	Bomb(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], bool, int, int, int);

	void update();
	void animate();
	void explode();

	Bomb& operator=(const Bomb& other);

private:
	void propogate(int, int);

	int distance;
	bool remote; // Checks if player has remote power up
};