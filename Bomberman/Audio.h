#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#pragma once

class Audio
{
public:
	Audio();

	void playSound(const std::string&);

	sf::Music& getMusic(const std::string&);
	sf::SoundSource::Status getStatus(const std::string&);

private:
	std::unordered_map<std::string, sf::Music*> music;
	std::unordered_map<std::string, sf::SoundBuffer> buffers;
	std::vector<sf::Sound> activeSounds;

	sf::Music titleMusic;
	sf::Music mainMusic;
	sf::Music powerUpMusic;
	sf::Music bonusStageMusic;
	sf::Music specialPowerUpMusic;
	sf::Music endingMusic;

	/*
		Music + sound names as follows:

		Music:
			"title"
			"main"
			"powerUp"
			"bonusStage"
			"specialPowerUp"
			"ending"

		Sounds:
			"roundStart"
			"stageClear"
			"miss"
			"gameOver"
	*/
};