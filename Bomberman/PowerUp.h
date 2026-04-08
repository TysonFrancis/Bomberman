#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Entity;
class Player;

/*
	PowerUp class to represent the different
	powerups that can be found in the game,
	as well as apply their effects to player.
*/

class PowerUp
{
public:
	enum class Type { ExtraBomb, ExtraRange, Skate, WallPhase, Remote, BombPhase, FireShield, Invincible };

	PowerUp(const sf::Texture&, Type, int, int);

	void applyEffect(Player&);
	bool intersects(const Entity&) const;

	sf::Sprite& getSprite();
	Type getType() const;

	friend std::ostream& operator<<(std::ostream&, const PowerUp&);
	PowerUp& operator=(const PowerUp&);

private:
	sf::Sprite sprite;
	Type type;
};