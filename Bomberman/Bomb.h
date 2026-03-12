#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Pod.h"
#include "Explosion.h"
#include "Constants.h"

class Bomb : public Entity
{
public:
	Bomb(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols],
		std::vector<Explosion>&, bool, int, int, int);

	void update();
	void animate();
	void explode();

	friend std::ostream& operator<<(std::ostream&, const Bomb&);
	Bomb& operator=(const Bomb&);

private:
	void propogate(int, int);

	std::vector<Explosion>& explosions;

	int distance;
	bool remote; // Checks if player has remote power up
	bool shrink;
};