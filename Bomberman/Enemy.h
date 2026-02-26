#pragma once
#include "Entity.h"
#include "Constants.h"
#include "Pod.h"

using namespace Constants;

class Enemy : public Entity
{
public:
	enum class Type
	{
		Ballom,
		Onil,
		Dahl,
		Minvo,
		Doria,
		Ovape,
		Pass,
		Pontan
	};

	Enemy(const sf::Texture&, Pod(&pod)[_rows][_cols], Type);

	void update();
	void die();

private:
	Pod (&pods)[_rows][_cols];
	Type type;
	float speed;
	int direct;
};