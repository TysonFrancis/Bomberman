#include "BonusPoint.h"
#include <cmath>
#include <iostream>

using namespace Constants;


BonusPoint::BonusPoint(const sf::Texture& tex, std::pair<int, int> pos, Bonus bonus) :
	PowerUp(tex, Type::BonusPoints, pos.first, pos.second), bonus(bonus) 
{
	sprite.setTextureRect(sf::IntRect({ static_cast<int>(bonus) * _tileSize, 16 }, _tile));
}

void BonusPoint::applyEffect(int& score)
{
	switch (bonus)
	{
	case Bonus::BPanel:		score += 10'000;		break;
	case Bonus::Goddess:	score += 20'000;		break;
	case Bonus::Cola:		score += 30'000;		break;
	case Bonus::Famicom:	score += 500'000;		break;
	case Bonus::Nakamoto:	score += 10'000'000;	break;
	case Bonus::Dezeniman:	score += 20'000'000;	break;
	}
}