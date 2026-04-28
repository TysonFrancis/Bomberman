#pragma once
#include <SFML/Graphics.hpp>
#include "PowerUp.h"
#include "Constants.h"

class BonusPoint : public PowerUp
{
public:
	enum class Bonus { BPanel, Goddess, Cola, Famicom, Nakamoto, Dezeniman };

	BonusPoint(const sf::Texture&, std::pair<int, int>, Bonus);
	
	void applyEffect(int& );

private:
	Bonus bonus;
};

