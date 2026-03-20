#pragma once
#include <SFML/Graphics.hpp>

namespace Constants
{
	// Array definition
	static constexpr int _rows = 13;
	static constexpr int _cols = 31;

	// Scaling
	static constexpr float _scale = 3.f;

	static constexpr int _tileSize = 16;
	static constexpr int _scaledTile = static_cast<int>(_tileSize * _scale);
	static constexpr int _halfTile = 8;
	static constexpr int _halfScaled = static_cast<int>(_halfTile * _scale);

	// Window size
	static constexpr unsigned int _windowWidth = _cols * _scaledTile;
	static constexpr unsigned int _windowHeight = _rows * _scaledTile;

	// Animations
	static constexpr int _fps = 60;
	static constexpr int _moveFrames = 3;
														// Player
	static constexpr int _playerDeathY = 32;
	static constexpr int _playerDeathFrames = 7;
	static constexpr int _playerTickSpeed = 5;
														// Enemies
	static constexpr int _enemyStartY = 240;
	static constexpr int _enemyDeathX = 96;
	static constexpr int _enemyColorDeathX = 112;
	static constexpr int _enemyColorDeathFrames = 4;
	static constexpr int _enemyTickSpeed = 10;
														// Explosions
	static constexpr int _explosionOffset = 80;
	static constexpr int _explosionStartX = 32;
	static constexpr int _explosionStartY = 96;
	static constexpr int _explosionTickSpeed = 6;
														// Bombs
	static constexpr int _bombY = 48;
	static constexpr int _bombTickSpeed = 15;
	static constexpr float _bombTimer = 2.5f;
														// SoftWalls
	static constexpr int _wallTickSpeed = 6;
														// Misc
	static constexpr sf::Vector2i _tile({ _tileSize, _tileSize });
	static constexpr sf::IntRect _emptyFrame({ 0, 0 }, { 0, 0 });
};