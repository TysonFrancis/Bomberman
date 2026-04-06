#include "PowerUp.h"

using namespace Constants;

PowerUp::PowerUp(const sf::Texture& tex, Pod(&pods)[Constants::_rows][Constants::_cols], Type input, int x, int y) :
	Entity(tex, pods), type(input)
{
	setTexture(static_cast<int>(type) * _tileSize, 0);
	setPosition(x, y);
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


// *** Public debugging method *** //

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
