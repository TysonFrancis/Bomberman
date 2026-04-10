#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "PowerUp.h"
#include "Text.h"
#include "Constants.h"

class InfoPanel : public sf::Drawable
{
public:
	InfoPanel(const sf::Texture&);
	~InfoPanel();

	void update();

	void updatePowerUp(PowerUp::Type);

	void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
	sf::RectangleShape backdrop;

	std::vector<sf::Sprite> powerUps;

	std::vector<Text*> texts;
};