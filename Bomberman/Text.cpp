#include "Text.h"

Text::Text(string text, int color, sf::Vector2f position)
{
	int colorOffset;
	sf::Vector2i glyphPosition;

	// load texture from file
	if (!texture.loadFromFile("Textures/Title.png"))
		std::cerr << "Error loading text glyphs from Title.png!";

	// choose color
	switch (color)
	{
	case 0:
		colorOffset = 16;	// black text
		break;
	case 1:
	default:
		colorOffset = 0;	// gray text (default)
	}

	// validate text input - TODO: allow cursor character
	for (int i = 0; i < text.size(); i++)				// for every character to display
	{
		if (text.at(i) > 64 && text.at(i) < 91)			// if character is uppercase between A-Z
			text.at(i) = tolower(text.at(i));			// lowercase it
		else if (text.at(i) < 97 || text.at(i) > 122)	// else if character is not lowercase between a-z
			if (text.at(i) < 48 || text.at(i) > 57)		// and NaN
				if (text.at(i) != 62)					// and not '>'
					text.erase(i--);					// remove it, decrementing i to avoid skipping chars
	}

	// choose glyph
	glyphPosition = {0, 0};
	
	// create sprite
	sprites.push_back(new sf::Sprite(texture));
	sprites.at(sprites.size() - 1)->setTextureRect(sf::IntRect(glyphPosition, sf::Vector2i(8, 8)));

	// position sprite
	position.x;
	position.y;
}


Text::~Text()
{
	for (int i = 0; i < sprites.size(); i++)
		delete sprites.at(i);
}