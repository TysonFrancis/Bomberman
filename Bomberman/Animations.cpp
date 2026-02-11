#include "Animations.h"
#include <iostream>

Animations::Animations()
{
	if (!entities.loadFromFile("Textures/Entities.png"))
		std::cerr << "Error loading from file Entities.png!\n";
	if (!background.loadFromFile("Textures/Background.png"))
		std::cerr << "Error loading from file Background.png!\n";
	if (!misc.loadFromFile("Textures/Misc.png"))
		std::cerr << "Error loading from file Misc.png!\n";
	if (!title.loadFromFile("Textures/Title.png"))
		std::cerr << "Error loading from file Title.png!\n";
	if (!icon.loadFromFile("Textures/Bomb Icon.png"))
		std::cerr << "Error loading from file Bomb Icon.png!\n";
}

sf::Texture& Animations::getEntities() { return entities; }
sf::Texture& Animations::getBackground() { return background; }
sf::Texture& Animations::getMisc() { return misc; }
sf::Texture& Animations::getTitle() { return title; }
sf::Image& Animations::getIcon() { return icon; }