#pragma once
#include "Entity.h"
#include "Constants.h"
#include "Pod.h"
#include "player.h"

class Enemy : public Entity
{
public:
	enum class Type { Ballom, Onil, Dahl, Minvo, Doria, Ovape, Pass, Pontan };

	Enemy(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], Type,Player(&play));

	void update();
	void animate();
	void die();
	Type getType() { return type; };

	friend std::ostream& operator<<(std::ostream&, const Enemy&);
	Enemy& operator=(const Enemy&);

private:
	bool isObstructed(int, int, bool);
	void changeDirection(bool);
	void randomMove(bool);
	void chasePlayer(bool, bool,bool);
	bool lineOfSight(bool, bool);

	Player(&play);
	Type type;
	float speed;
	double moveX, moveY;
	int eratic;
	Entity::Facing lastFacing;
};