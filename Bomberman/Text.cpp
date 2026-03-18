#include "Text.h"

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
					text.erase(i--);					// remove it, decrementing i to avoid skipping chars
					continue;							// move on to next char
				}

		// choose glyph
		if (text.at(i) > 96 && text.at(i) < 123)				// if letter
			glyphPosition = { (text.at(i) - 97) * 8, 256 };		// get glyph from corresponding position
		else if (text.at(i) > 47 && text.at(i) < 58)			// if number
			glyphPosition = { (text.at(i) - 48) * 8, 248 };		// get glyph form corresponding position
		else													// should be > hopefully
			glyphPosition = { 80, 248 };						// get glyph from corresponding position

		// set color
		glyphPosition.y += isBlack * 16;	// more evil bool math >:D
	
		// create sprite
		sprites.push_back(new sf::Sprite(texture));												// create the actual sprite
		sprites.at(sprites.size() - 1)->setTextureRect(sf::IntRect(glyphPosition, { 8, 8 }));	// assign the determined glyph

		// position sprite
		sprites.at(i)->setPosition({ position.x + i * 8, position.y });
	}
}


Text::~Text()
{
	// delete all sprites of this object when text is done displaying
	for (int i = 0; i < sprites.size(); i++)
		delete sprites.at(i);
}