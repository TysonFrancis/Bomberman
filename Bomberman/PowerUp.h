#pragma once
#include "Entity.h"
#include "Player.h"

#include "Constants.h"

class PowerUp : public Entity
{
public:
	enum class Type { ExtraBomb, ExtraRange, Skate, WallPhase, Remote, BombPhase, FireShield, Invincible };

	PowerUp(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], Type, int, int);

	void applyEffect(Player&);

	friend std::ostream& operator<<(std::ostream&, const PowerUp&);

private:
	Type type;
};