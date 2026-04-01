#pragma once
#include "Entity.h"
#include "Constants.h"
#include "Pod.h"
#include "player.h"

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