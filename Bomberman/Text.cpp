#include "Text.h"
#include <iostream>

using namespace Constants;
using std::string;

const sf::Texture* Text::texture = nullptr;		// Set to nullptr outside of methods

Text::Text(string text, sf::Vector2f pos, int align, bool isBlack) :
	position(pos), align(align)
{
	construct(text, isBlack);
}

// Load texture once at startup in game constructor -> method is static for this
void Text::setTexture(const sf::Texture& tex) { texture = &tex; }

void Text::edit(const std::string& newText, bool isBlack)
{
	sprites.clear();
	construct(newText, isBlack);
}


// *** Private helper method *** //

void Text::construct(string text, bool isBlack)
{
	sf::Vector2i glyphPosition;

	float totalWidth = text.size() * _halfScaled, offset = 0;

	if (align == 0)
		offset -= totalWidth / 2.f;
	else if (align == 1)
		offset -= totalWidth;

	for (int i = 0; i < text.size(); i++)							// for every character to display
	{
		if (text.at(i) > 96 && text.at(i) < 123)						// if letter
			glyphPosition = { (text.at(i) - 97) * _halfTile, _letterY };	// get letter
		else if (text.at(i) > 47 && text.at(i) < 58)					// if number
			glyphPosition = { (text.at(i) - 48) * _halfTile, _numberY };	// get number
		else if (text.at(i) == 62)										// if '>'
			glyphPosition = { _arrowX, _numberY };							// get glyph
		else															// if space (or anything else somehow)
			glyphPosition = _spaceCharacter;								// put a colored square for skipping

		glyphPosition.y += isBlack * _tileSize;									// Set color

		auto& sprite = sprites.emplace_back(*texture);								// Create the actual sprite
		sprite.setTextureRect(sf::IntRect(glyphPosition, _letterTile));				// Assign the determined glyph
		sprite.setScale({ _scale, _scale });										// scale sprite to match others
		sprite.setPosition({ position.x + i * _halfScaled + offset, position.y });	// position sprite
	}
}