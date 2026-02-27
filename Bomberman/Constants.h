#pragma once

namespace Constants
{
	// Array definition
	static constexpr int _rows = 13;
	static constexpr int _cols = 31;

	// Scaling
	static constexpr float _scale = 3.f;
	static constexpr int _tileSize = 16;
	static constexpr int _scaledTile = _tileSize * _scale;

	// Window size
	static constexpr unsigned int _windowWidth = _cols * _scaledTile;
	static constexpr unsigned int _windowHeight = _rows * _scaledTile;

	// Animation stuff, most likely more to come maybe
	static constexpr unsigned int _fps = 60;
};