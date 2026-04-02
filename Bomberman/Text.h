#pragma once
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"

using std::string;

/*
* Other things to consider adding:
* - Selectable left/right/center and top/bottom alignment for easier positioning
* - Optional lifespan argument to automatically remove text later
*/


/* Displays text using font textures in the center of the screen.
Only supports A-Z, 0-9, or '>'. Displays in caps regardless of input.*/
class Text
{
public:
	/*
		@param std::string: the text to display on screen
		@param int: the color of the text's drop shadow - 0 for black, 1 for gray
		@param sf::Vector2f: The position the text should appear in - use -1 for center value
	*/
	Text(string, sf::Vector2f, bool = false);
	~Text();

	std::vector<sf::Sprite*> sprites;

private:
	sf::Texture texture;
};