#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Pod.h"
#include "Explosion.h"
#include "Animations.h"
#include "Constants.h"

class Bomb : public Entity
{
public:
	Bomb(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols],
		std::vector<Explosion>&, bool, int, int, int, const Animations&);

	void update();
	void animate();
	void explode();
	void delay();

	bool getWillExplode();

	friend std::ostream& operator<<(std::ostream&, const Bomb&);
	Bomb& operator=(const Bomb&);
private:
	void propogate(int, int);

	const Animations& frames;

	std::vector<Explosion>& explosions;

	int distance;
	bool remote; // Checks if player has remote power up
	bool shrink;
	int now; // Used to delay explosion 

	bool willExplode = false; //Used to delay explosion 
};