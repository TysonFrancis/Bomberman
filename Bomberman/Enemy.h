#pragma once
#include "Entity.h"
#include "Constants.h"

class Player;
class Pod;

/*
	Enemy class, represents all enemy types, distiguished by enum Type.
	Has different movement patterns and speed based on type.
*/

class Enemy : public Entity
{
public:
	enum class Type { Ballom, Onil, Dahl, Minvo, Doria, Ovape, Pass, Pontan };

	Enemy(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], Type, Player(&play));

	void update();
	void animate();
	void die();

	Type getType() const;

	friend std::ostream& operator<<(std::ostream&, const Enemy&);
	Enemy& operator=(const Enemy&);

	int getX() { return tileX; }
	int getY() { return tileY; }

private:
	void randomMove(bool);
	void chasePlayer(bool, bool, bool);
	void changeDirection(bool);
	bool lineOfSight(bool, bool);
	bool isObstructed(int, int, bool);
	bool sightObstruction(int, int, bool);

	Player(&play);

	Type type;
	Facing lastFacing;

	float moveX, moveY;

	float speed;
	int eratic;

	int enemyFrameYPos;
};