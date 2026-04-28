#pragma once
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>

#include "PowerUp.h"
#include "Text.h"
#include "Constants.h"

class Player;

class InfoPanel : public sf::Drawable
{
public:
	InfoPanel(const sf::Texture&, const sf::Texture&, const Player&);

	void update();
	void updateLives(bool = false);
	void updatePowerUp(PowerUp::Type, bool = true);

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
	sf::RectangleShape backdrop;
	Text timer, score;
	std::optional<Text>bombCount, bombRange;
	std::vector<sf::Sprite> powerUps;
	std::vector<sf::Sprite> lives;

	sf::Texture lifeTex;

	int giveBomb;
	int giveRange;
};