#include "Text.h"

using namespace Constants;


// Overload shenanigans
Text::Text(string text, sf::Vector2f position, int align, bool isBlack)
{
	construct(text, position, align, isBlack);
}

// ..
Text::Text(string text, sf::Vector2f position, bool isBlack)
{
	construct(text, position, isBlack, 0);
}


// Actual construct code
void Text::construct(string text, sf::Vector2f position, int align, bool isBlack)
{
	sf::Vector2i glyphPosition;
	int offset;

	// load texture from file
	if (!texture.loadFromFile("Textures/Title.png"))
		std::cerr << "Error loading text glyphs from Title.png!";

	// calculate offset for alignment
	offset = 8 * _scale * text.size() / 2 * align;

	for (int i = 0; i < text.size(); i ++)	// for every character to display
	{
		// choose glyph
		if (text.at(i) > 96 && text.at(i) < 123)				// if letter
			glyphPosition = { (text.at(i) - 97) * _halfTile, _letterY };			// get glyph from corresponding position
		else if (text.at(i) > 47 && text.at(i) < 58)			// if number
			glyphPosition = { (text.at(i) - 48) * _halfTile, _numberY };			// get glyph form corresponding position
		else if (text.at(i) == 62)								// if '>'
			glyphPosition = { _arrowX, _numberY };							// get glyph from corresponding position
		else													// if space (or anything else somehow)
			glyphPosition = { 0, 0 };								// put a black squre because skipping
																	// sprite creation causes too many issues 

		// set color
		glyphPosition.y += isBlack * _tileSize;	// more evil bool math >:D

		// create sprite
		sprites.push_back(new sf::Sprite(texture));									// create the actual sprite
		sprites.back()->setTextureRect(sf::IntRect(glyphPosition, _letterTile));	// assign the determined glyph
		sprites.back()->setScale({ _scale, _scale });								// scale sprite to match others

		// position sprite
		sprites.at(i)->setPosition({ position.x + i * _halfScaled + offset, position.y});
	}
}


Text::~Text()
{
	// delete all sprites of this object when text is done displaying
	for (int i = 0; i < sprites.size(); i++)
		delete sprites.at(i);
}