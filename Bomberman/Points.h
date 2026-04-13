#pragma once
#include "Entity.h"
#include "Constants.h"

class Player;
class Pod;


class Points: public Entity
{
public:

	Points(const sf::Texture&, 
		Pod(&pods)[Constants::_rows][Constants::_cols], int, int, int);
	void update();

	Points& operator=(const Points&);

private:
	int value;
	int x, y;
};