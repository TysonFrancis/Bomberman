#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "PowerUp.h"
#include "Text.h"
#include "Constants.h"

class InfoPanel : public sf::Drawable
{
public:
	InfoPanel(const sf::Texture&, const sf::Texture&, const sf::Texture&);

	void update();
	void updateLives(bool = false);
	void updatePowerUp(PowerUp::Type, bool = true);

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
	sf::RectangleShape backdrop;
	std::vector<Text> texts;
	std::vector<sf::Sprite> powerUps;
	std::vector<sf::Sprite> lives;

	sf::Texture lifeTex;
};