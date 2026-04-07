#pragma once
#include "Entity.h"
#include "Player.h"

#include "Constants.h"

class PowerUp
{
public:
	enum class Type { ExtraBomb, ExtraRange, Skate, WallPhase, Remote, BombPhase, FireShield, Invincible };

	PowerUp(const sf::Texture&, Pod(&pods)[Constants::_rows][Constants::_cols], Type, int, int);

	void applyEffect(Player&);

	sf::Sprite& getSprite();
	Type getType() const;

	friend std::ostream& operator<<(std::ostream&, const PowerUp&);
	PowerUp& operator=(const PowerUp&);

private:
	sf::Sprite sprite;
	Type type;
};