#pragma once
#include <SFML/System/Vector2.hpp>

/*
	Constants namespace to hold all repeated values
	for the game, such as window size, animation speeds,
	and other values that are used in multiple places.

	This is to avoid magic numbers and make it
	easier to change values in one place if needed.
*/

namespace Constants
{
	// Array definition
	static constexpr int _rows = 13;
	static constexpr int _cols = 31;
	static constexpr int _softPods = 249;

	// Scaling
	static constexpr float _scale = 3.f;
	static constexpr float _speedScale = _scale / 3.f;		// 3x is base scale that we are finding speeds at, use that as divisor

	static constexpr int _tileSize = 16;
	static constexpr int _scaledTile = static_cast<int>(_tileSize * _scale);
	static constexpr int _halfTile = 8;
	static constexpr int _halfScaled = static_cast<int>(_halfTile * _scale);
	static constexpr int _quarterTile = 4;
	static constexpr int _quarterScaled = static_cast<int>(_quarterTile * _scale);

	// Window size
	static constexpr unsigned int _windowWidth = _cols * _scaledTile;
	static constexpr unsigned int _windowHeight = _rows * _scaledTile + 2 * _scaledTile ;

	// Highscore positions
	static constexpr sf::Vector2f _highscoreTitlePosition({ 912, 504 });
	static constexpr sf::Vector2f _highscoreGameoverPosition({ 888, 624 });

	// Animations
	static constexpr int _fps = 60;
	static constexpr int _moveFrames = 3;
																	// Player
	static constexpr int _playerDeathY = 32;
	static constexpr int _playerDeathFrames = 7;
	static constexpr float _playerSpeed = 4.f;
	static constexpr int _playerTickSpeed = 5;
																	// Enemies
	static constexpr int _enemyDeathX = 96;
	static constexpr int _enemyLeftX = 48;
	static constexpr int _enemyStartY = 240;
	static constexpr int _enemyColorDeathX = 112;
	static constexpr int _enemyColorDeathFrames = 4;
	static constexpr int _enemyTickSpeed = 10;
																	// Explosions
	static constexpr int _explosionOffset = 80;
	static constexpr int _explosionStartX = 32;
	static constexpr int _explosionStartY = 96;
	static constexpr int _explosionFrames = 4;
	static constexpr int _explosionTotalFrames = 7;
	static constexpr int _explosionTickSpeed = 6;
																	// Bombs
	static constexpr int _bombY = 48;
	static constexpr int _bombTickSpeed = 15;
	static constexpr int _bombFrames = 3;
	static constexpr int _bombDelay = 5;
	static constexpr float _bombTimer = 2.5f;
																	// SoftWalls
	static constexpr int _softWallX = 64;
	static constexpr int _softWallY = 48;
	static constexpr int _softWallDeathFrames = 6;
	static constexpr int _wallTickSpeed = 6;
																	// Misc
	static constexpr int _exitX = 176;										// Exit
	static constexpr int _exitY = 48;

	static constexpr int _arrowX = 80;										// Letters
	static constexpr int _numberY = 248;
	static constexpr int _letterY = 256;

	static constexpr int _bonusTimer = _fps * 30;							// Timers
	static constexpr int _invincibilityTimer = _fps * 30;
	static constexpr int _pontanTimer = _fps * 200;

	static constexpr int _slowBlinkSpeed = _invincibilityTimer - 6 * _fps;	// Blinker controls
	static constexpr int _slowBlinkInterval = 20;
	static constexpr int _mediumBlinkSpeed = _invincibilityTimer - 3 * _fps;
	static constexpr int _mediumBlinkInterval = 10;
	static constexpr int _fastBlinkSpeed = _invincibilityTimer - static_cast<int>(1.5 * _fps);
	static constexpr int _fastBlinkInterval = 4;

	static constexpr sf::Vector2i _spaceCharacter({ 88, 248 });				// Position vector

	static constexpr sf::Vector2i _tile({ _tileSize, _tileSize });			// Size vectors
	static constexpr sf::Vector2i _letterTile({ _halfTile, _halfTile });

	static constexpr sf::Vector2f _centerScreen({ _windowWidth / 2.f, _windowHeight / 2.f });
};