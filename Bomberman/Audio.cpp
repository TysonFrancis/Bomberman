#include "Audio.h"
#include <iostream>

Audio::Audio()
{
    // Load music files
	if (!titleMusic.openFromFile("Audio/01. Title Screen.mp3"))
		std::cerr << "Error opening from file \"01. Title Screen.mp3\"!\n";
	if(!mainMusic.openFromFile("Audio/03. Main BGM.mp3"))
		std::cerr << "Error opening from file \"03. Main BGM.mp3\"!\n";
	if(!powerUpMusic.openFromFile("Audio/04. Power-Up Get.mp3"))
		std::cerr << "Error opening from file \"04. Power-Up Get.mp3\"!\n";
	if (!bonusStageMusic.openFromFile("Audio/06. Bonus Stage.mp3"))
		std::cerr << "Error opening from file \"06. Bonus Stage.mp3\"!\n";
	if (!specialPowerUpMusic.openFromFile("Audio/07. Special Power-Up Get.mp3"))
		std::cerr << "Error opening from file \"07. Special Power-Up Get.mp3\"!\n";
    if(!endingMusic.openFromFile("Audio/08. Ending.mp3"))
		std::cerr << "Error opening from file \"08. Ending.mp3\"!\n";

	music["title"] = &titleMusic;                      // Add music to map with corrosponding name for easier access
	music["main"] = &mainMusic;
	music["powerUp"] = &powerUpMusic;
	music["bonusStage"] = &bonusStageMusic;
	music["specialPowerUp"] = &specialPowerUpMusic;
	music["ending"] = &endingMusic;

    struct SoundFile { std::string name, path; };           // Struct to hold sound name and path for easier loading

	SoundFile sounds[] =                                    // List of sound effects to load with name corrosponding to file path
    {
        {"roundStart", "Audio/02. Stage Start.flac"},
        {"stageClear", "Audio/05. Stage Clear.flac"},
        {"miss", "Audio/09. Miss.flac"},
        {"gameOver", "Audio/10. Game Over.flac"}
    };

    for (SoundFile& sound : sounds)                         // For each sound in array
    {
        sf::SoundBuffer buffer;                                 // Make a default buffer
		if (!buffer.loadFromFile(sound.path))                       // Load sound into buffer
            std::cerr << "Failed to load \"" << sound.path << "\"!\n";
		else                                                        // Add buffer to map with corrosponding name
            buffers[sound.name] = buffer;
    }
}

// Plays the sound selected by name parameter, if it exists,
// and adds it to the active sounds vector to keep it alive while playing
void Audio::playSound(const std::string& name)
{
	// Clean up stopped sounds before adding a new one,
    // to prevent the vector from growing indefinitely
    for (size_t i = 0; i < activeSounds.size(); i++)
        if (activeSounds[i].getStatus() == sf::SoundSource::Status::Stopped)
        {
            activeSounds.erase(activeSounds.begin() + i);
            i--;
        }

	if (buffers.find(name) != buffers.end())            // Check if the sound buffer exists
    {
		activeSounds.emplace_back(buffers.find(name)->second);      // Add a new sound to the active sounds vector with the requested buffer
		activeSounds.back().play();                                 // Play the sound
    }
	else
		throw std::runtime_error("Sound \"" + name + "\" not found!");		// Throw error if sound doesn't exist
}

// Returns the music selected by name parameter if it exists
sf::Music& Audio::getMusic(const std::string& name)
{
	if (music.find(name) != music.end())                 // Check if the music exists
		return *music.find(name)->second;							// Return the music
	throw std::runtime_error("Music \"" + name + "\" not found!");			// Throw error if music doesn't exist
}

// Returns the status of the sound selected by name parameter if it exists
sf::SoundSource::Status Audio::getStatus(const std::string& name)
{
	if (buffers.find(name) != buffers.end())                    // Check if the sound buffer exists
    {
		for (const sf::Sound& sound : activeSounds)                 // Iterate through active sounds
			if (&sound.getBuffer() == &buffers.find(name)->second)      // If the sound's buffer matches the requested buffer, return its status
                return sound.getStatus();
    }

    return sf::SoundSource::Status::Stopped;                    // Default to stopped if not found
}