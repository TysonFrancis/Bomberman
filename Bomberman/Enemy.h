#pragma once
#include "Entity.h"
#include "Constants.h"
#include "Pod.h"

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

	Enemy(const sf::Texture&, Pod(&pod)[Constants::_rows][Constants::_cols], Type);

	void update();
	void animate();
	void die();

	// Enemy display, will show type and other info if needed
	friend std::ostream& operator<<(std::ostream&, const Enemy&);

	Enemy& operator=(const Enemy& other);

private:
	Pod (&pods)[Constants::_rows][Constants::_cols];
	Type type;
	float speed;
	int direct;
};