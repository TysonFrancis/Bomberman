#include <SFML/Audio.hpp>
#pragma once

class Audio
{
public:
	Audio();

	sf::Music& getRoundStart();

private:
	// SOUND
	// (empty)
	
	// MUSIC
	sf::Music roundStart;
};