#include "Audio.h"
#include <iostream>

Audio::Audio()
{
	if (!roundStart.openFromFile("Audio/02. Stage Start.flac"))
		std::cerr << "Error opening from file \"02. Stage Start.flac\"!\n";
}


sf::Music& Audio::getRoundStart() { return roundStart; }