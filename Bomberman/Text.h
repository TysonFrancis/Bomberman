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
Only supports a-z, 0-9, or '>'. LOWERCASE ONLY.*/
class Text
{
public:
	/*
		@param std::string: The text to display on screen
		@param sf::Vector2f: The position the text should appear in
		@param int (optional): The text's alignment - -1 for left, 0 for center, 1 for right
		@param bool (optional): The color of the text's drop shadow - true for black. Defaults false, for gray.
	*/
	Text(string, sf::Vector2f, int = -1, bool = false);
	Text(string, sf::Vector2f, bool);
	~Text();

	std::vector<sf::Sprite*> sprites;

private:
	void construct(string, sf::Vector2f, int, bool);

	sf::Texture texture;
};