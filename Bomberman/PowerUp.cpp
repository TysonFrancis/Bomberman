#include "PowerUp.h"
#include "Player.h"

using namespace Constants;

PowerUp::PowerUp(const sf::Texture& tex, Type input, int x, int y) :
	sprite(tex), type(input)
{
	sprite.setTextureRect(sf::IntRect({ static_cast<int>(type) * _tileSize, 0 }, _tile));
	sprite.setOrigin(sf::Vector2f(_halfTile, _halfTile));
	sprite.setScale(sf::Vector2f(_scale, _scale));
	sprite.setPosition(sf::Vector2f(x * _scaledTile + _halfScaled, y * _scaledTile + _halfScaled));
}

void PowerUp::applyEffect(Player& bomber)
{
	switch (type)
	{
	case Type::ExtraBomb:	bomber.extraBomb();		break;
	case Type::ExtraRange:	bomber.extraRange();	break;
	case Type::Skate:		bomber.giveSkate();		break;
	case Type::WallPhase:	bomber.phaseWalls();	break;
	case Type::Remote:		bomber.giveRemote();	break;
	case Type::BombPhase:	bomber.phaseBombs();	break;
	case Type::FireShield:	bomber.shieldFire();	break;
	case Type::Invincible:	bomber.invincible();	break;
	}
}

bool PowerUp::intersects(const Entity& entity) const
{
	return sprite.getGlobalBounds().findIntersection(entity.getSprite().getGlobalBounds()).has_value();
}

sf::Sprite& PowerUp::getSprite()		{ return sprite; }
PowerUp::Type PowerUp::getType() const	{ return type; }


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& out, const PowerUp& powerUp)
{
	out << "PowerUp get: ";

	switch (powerUp.type)
	{
	case PowerUp::Type::ExtraBomb:	out << "extraBomb";		break;
	case PowerUp::Type::ExtraRange:	out << "extraRange";	break;
	case PowerUp::Type::Skate:		out << "skate";			break;
	case PowerUp::Type::WallPhase:	out << "wallPhase";		break;
	case PowerUp::Type::Remote:		out << "remote";		break;
	case PowerUp::Type::BombPhase:	out << "bombPhase";		break;
	case PowerUp::Type::FireShield:	out << "fireShield";	break;
	case PowerUp::Type::Invincible:	out << "invincible";	break;
	}

	out << "\n";

	return out;
}

PowerUp& PowerUp::operator=(const PowerUp& other)
{
	if (this != &other)
	{
		sprite = other.sprite;
		type = other.type;
	}

	return *this;
}