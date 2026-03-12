#pragma once
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"

using std::string;


/* Displays text using font textures in the center of the screen.
Only supports A-Z. Displays in caps regardless of input.*/
class Text
{
public:
	/*
		@param std::string: the text to display on screen
		@param int: the color of the text's drop shadow - 0 for black, 1 for gray
		@param sf::Vector2f: The position the text should appear in - use -1 for center value
	*/
	Text(string, int, sf::Vector2f);
	~Text();

private:
	sf::Texture texture;
	std::vector<sf::Sprite*> sprites;
};