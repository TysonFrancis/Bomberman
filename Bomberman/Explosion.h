#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Pod.h"
#include "Constants.h"

class Explosion : public Entity
{
public:
	Explosion(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols],
		int, int, Facing, bool);

	void update();
	void animate();
	void die();

	friend std::ostream& operator<<(std::ostream&, const Explosion&);
	Explosion& operator=(const Explosion&);

private:
	void setTexture();

	int row;
	bool end;
	bool shrink;
};
