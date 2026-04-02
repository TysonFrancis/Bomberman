#include "Text.h"

using namespace Constants;


Text::Text(string text, sf::Vector2f position, bool isBlack)
{
	sf::Vector2i glyphPosition;

	// load texture from file
	if (!texture.loadFromFile("Textures/Title.png"))
		std::cerr << "Error loading text glyphs from Title.png!";

	for (int i = 0; i < text.size(); i++)	// for every character to display
	{
		// validate text input
		if (text.at(i) > 64 && text.at(i) < 91)			// if character is uppercase between A-Z
			text.at(i) = tolower(text.at(i));			// lowercase it
		else if (text.at(i) < 97 || text.at(i) > 122)	// else if character is not lowercase between a-z
			if (text.at(i) < 48 || text.at(i) > 57)		// and NaN
				if (text.at(i) != 62)					// and not '>'
				{
					text.erase(i--, 1);					// remove it, decrementing i to avoid skipping chars
					continue;							// move on to next char
				}

		// choose glyph
		if (text.at(i) > 96 && text.at(i) < 123)								// if letter
			glyphPosition = { (text.at(i) - 97) * _halfTile, _letterY };		// get glyph from corresponding position
		else if (text.at(i) > 47 && text.at(i) < 58)							// if number
			glyphPosition = { (text.at(i) - 48) * _halfTile, _numberY };		// get glyph form corresponding position
		else																	// should be > hopefully
			glyphPosition = { _arrowX, _numberY };								// get glyph from corresponding position

		// set color
		glyphPosition.y += isBlack * _tileSize;	// more evil bool math >:D

		// create sprite
		sprites.push_back(new sf::Sprite(texture));									// create the actual sprite
		sprites.back()->setTextureRect(sf::IntRect(glyphPosition, _letterTile));	// assign the determined glyph
		sprites.back()->setScale({ _scale, _scale });

		// position sprite
		sprites.at(i)->setPosition({ position.x + i * _halfScaled, position.y});
	}
}


Text::~Text()
{
	// delete all sprites of this object when text is done displaying
	for (int i = 0; i < sprites.size(); i++)
		delete sprites.at(i);
}