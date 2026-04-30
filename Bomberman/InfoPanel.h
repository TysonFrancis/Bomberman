#pragma once
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>

#include "PowerUp.h"
#include "Text.h"
#include "Constants.h"

class InfoPanel : public sf::Drawable
{
public:
	InfoPanel(const sf::Texture&, const sf::Texture&);

	void update();
	void updateLives(bool = false);
	void updatePowerUp(PowerUp::Type, bool = true);

	void reset();

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
	void setPowerUps();
	void setLives();

	sf::RectangleShape backdrop;

	Text timer, score;
	std::optional<Text> bombCount, bombRange;

	std::vector<sf::Sprite> powerUps;
	std::vector<sf::Sprite> lives;

	const sf::Texture powerUpTex;
	const sf::Texture lifeTex;

	int giveBomb;
	int giveRange;
};