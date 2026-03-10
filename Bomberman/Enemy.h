#pragma once
#include "Entity.h"
#include "Constants.h"
#include "Pod.h"

class Enemy : public Entity
{
public:
	enum class Type { Ballom, Onil, Dahl, Minvo, Doria, Ovape, Pass, Pontan };

	Enemy(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], Type);

	void update();
	void animate();
	void die();

	friend std::ostream& operator<<(std::ostream&, const Enemy&);
	Enemy& operator=(const Enemy& other);

private:
	bool isObstructed(int, int);
	void changeDirection();

	Type type;
	float speed;
	double moveX, moveY;
	Entity::Facing lastFacing;
};