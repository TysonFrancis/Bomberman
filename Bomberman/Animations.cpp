#include "Animations.h"

Animations::Animations()
{
	if (!entities.loadFromFile("Textures/Entities.png"))
		std::cerr << "Error loading from file Background.png!\n";
	if (!background.loadFromFile("Textures/Background.png"))
		std::cerr << "Error loading from file Background.png!\n";
	if (!misc.loadFromFile("Textures/Misc.png"))
		std::cerr << "Error loading from file Misc.png!\n";
	if (!title.loadFromFile("Textures/Entities.png"))
		std::cerr << "Error loading from file Title.png!\n";
}

sf::Texture Animations::getEntities() { return entities; }
sf::Texture Animations::getBackground() { return background; }
sf::Texture Animations::getMisc() { return misc; }
sf::Texture Animations::getTitle() { return title; }


void Animations::draw(sf::RenderWindow& window) { window.draw(sprite); }
void Animations::setPosition(float x, float y) { sprite.setPosition({ x, y }); }
void Animations::setScale(float x, float y) { sprite.setScale({ x, y }); }