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

const sf::Texture& Animations::getEntities() const  { return entities; }
const sf::Texture& Animations::getBackground() const  { return background; }
const sf::Texture& Animations::getMisc() const  { return misc; }
const sf::Texture& Animations::getTitle() const  { return title; }
const sf::Image& Animations::getIcon() const  { return icon; }