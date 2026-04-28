#include "Text.h"
#include <iostream>

using namespace Constants;
using std::string;

const sf::Texture* Text::texture = nullptr;		// Set to nullptr outside of methods

Text::Text(const string& text, sf::Vector2f pos, int align, bool isBlack, bool infoScale) :
	position(pos), align(align)
{
	construct(text, isBlack, infoScale);
}

// Load texture once at startup in game constructor -> method is static for this
void Text::setTexture(const sf::Texture& tex) { texture = &tex; }

void Text::edit(const string& newText, bool isBlack, bool infoScale)
{
	sprites.clear();
	construct(newText, isBlack, infoScale);
}


// *** Private helper method *** //

void Text::construct(string text, bool isBlack, bool infoScale)
{
	sf::Vector2i glyphPosition;

	float totalWidth = static_cast<float>(text.size() * _halfScaled);
	float offset = 0;

	if (align == 0)
		offset -= totalWidth / 2.f;
	else if (align == 1)
		offset -= totalWidth;

	for (size_t i = 0; i < text.size(); i++)							// for every character to display
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

		if (infoScale)																// If on info panel shrink by half
			sprite.setScale({ _scale / 2.f, _scale / 2.f });
		else																		// Else scale normally
			sprite.setScale({ _scale, _scale });

		if (infoScale)																// If on info panel move over by only half
			sprite.setPosition({ position.x + i * _quarterScaled + offset, position.y });
		else																		// Else position normally
			sprite.setPosition({ position.x + i * _halfScaled + offset, position.y });
	}
}