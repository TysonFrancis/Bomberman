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

	// Animation stuff, most likely more to come maybe
	static constexpr unsigned int _fps = 60;
	static constexpr unsigned int _moveFrames = 3;

	static constexpr int _explosionOffset = 80;
	static constexpr int _explosionStartX = 32;
	static constexpr int _explosionStartY = 96;
	static constexpr int _explosionTickSpeed = 6;

	static constexpr int _bombTickSpeed = 15;
	static constexpr float _bombTimer = 2.5;

	static constexpr int _wallTickSpeed = 6;

	static constexpr sf::Vector2i _tile({ _tileSize, _tileSize });
	static constexpr sf::IntRect _emptyFrame({ 0, 0 }, { 0, 0 });
};