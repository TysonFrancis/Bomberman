#include "Animations.h"
#include <iostream>

using Constants::_tile;

Animations::Animations()
{
	// Load textures from files, print error if failed
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

	// Set up explosion hash table
	setExplosionFrames();
}

const sf::Texture& Animations::getEntities() const		{ return entities; }
const sf::Texture& Animations::getBackground() const	{ return background; }
const sf::Texture& Animations::getMisc() const			{ return misc; }
const sf::Texture& Animations::getTitle() const			{ return title; }
const sf::Image&   Animations::getIcon() const			{ return icon; }

const std::vector<sf::IntRect>& Animations::getPlayerFrames(WalkingDirection dir) const
{
	return playerTable.at(dir);
}

const std::vector<sf::IntRect>& Animations::getEnemyFrames(WalkingDirection dir) const
{
	return enemyTable.at(dir);
}

// Return the vector of frames for the given explosion type

// Unordered maps work with keys and values, we already set the values
// to be vectors of frames for each explosion type in the constructor,
// so we just need to return the vector for the given explosion type key
const std::vector<sf::IntRect>& Animations::getExplosionFrames(ExplosionType type) const
{
	return explosionTable.at(type);		// Return vector of frames for given type
}

void Animations::setPlayerFrames()
{

}

void Animations::setEnemyFrames()
{

}

void Animations::setExplosionFrames()
{
	explosionTable[ExplosionType::Center] =		// For a center explosion frame, positions
	{											// for various frames are as follows,
		sf::IntRect({ 32, 96 }, _tile),				// Frame 1
		sf::IntRect({ 112, 96 }, _tile),			// Frame 2
		sf::IntRect({ 32, 176 }, _tile),			// Frame 3
		sf::IntRect({ 112, 176 }, _tile)			// Frame 4
	};

	explosionTable[ExplosionType::Up] =			// Continue with same pattern for each explosion type
	{
		sf::IntRect({ 32, 80 }, _tile),
		sf::IntRect({ 112, 80 }, _tile),
		sf::IntRect({ 32, 160 }, _tile),
		sf::IntRect({ 112, 160 }, _tile)
	};

	explosionTable[ExplosionType::Down] =
	{
		sf::IntRect({ 32, 112 }, _tile),
		sf::IntRect({ 112, 112 }, _tile),
		sf::IntRect({ 32, 192 }, _tile),
		sf::IntRect({ 112, 192 }, _tile)
	};

	explosionTable[ExplosionType::Left] =
	{
		sf::IntRect({ 16, 96 }, _tile),
		sf::IntRect({ 96, 96 }, _tile),
		sf::IntRect({ 16, 176 }, _tile),
		sf::IntRect({ 96, 176 }, _tile)
	};

	explosionTable[ExplosionType::Right] =
	{
		sf::IntRect({ 48, 96 }, _tile),
		sf::IntRect({ 128, 96 }, _tile),
		sf::IntRect({ 48, 176 }, _tile),
		sf::IntRect({ 128, 176 }, _tile)
	};

	explosionTable[ExplosionType::EndUp] =
	{
		sf::IntRect({ 32, 64 }, _tile),
		sf::IntRect({ 112, 64 }, _tile),
		sf::IntRect({ 32, 144 }, _tile),
		sf::IntRect({ 112, 144 }, _tile)
	};

	explosionTable[ExplosionType::EndDown] =
	{
		sf::IntRect({ 32, 128 }, _tile),
		sf::IntRect({ 112, 128 }, _tile),
		sf::IntRect({ 32, 208 }, _tile),
		sf::IntRect({ 112, 208 }, _tile)
	};

	explosionTable[ExplosionType::EndLeft] =
	{
		sf::IntRect({ 0, 96 }, _tile),
		sf::IntRect({ 80, 96 }, _tile),
		sf::IntRect({ 0, 176 }, _tile),
		sf::IntRect({ 80, 176 }, _tile)
	};

	explosionTable[ExplosionType::EndRight] =
	{
		sf::IntRect({ 64, 96 }, _tile),
		sf::IntRect({ 144, 96 }, _tile),
		sf::IntRect({ 64, 176 }, _tile),
		sf::IntRect({ 144, 176 }, _tile)
	};
}