#pragma once
#include <SFML/Graphics.hpp>
#include "Pod.h"
#include "Constants.h"
#include "Entity.h"

using namespace Constants;


class Bomb : public Entity
{
public:
	Bomb(const sf::Texture&, bool, int, Pod(&pods)[_rows][_cols], int, int);
	~Bomb();

	void update();
	void explode();
	void animate();

private:
	Pod(&pods)[_rows][_cols];
	int x, y;
	int distance;
	bool remote; //Checks if player has remote power up
};