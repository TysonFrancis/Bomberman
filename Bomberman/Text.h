#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Constants.h"

/*
	Text class, displays text using font
	textures in the center of the screen.
	Only supports a-z, 0-9, or '>'. LOWERCASE ONLY.
*/

/*
* Other things to consider adding:
* - Selectable left/right/center and top/bottom alignment for easier positioning
* - Optional lifespan argument to automatically remove text later
*/

class Text
{
public:
	/*
		@param std::string: The text to display on screen
		@param sf::Vector2f: The position the text should appear in
		@param int (optional): The text's alignment - -1 for left, 0 for center, 1 for right
		@param bool (optional): The color of the text's drop shadow - true for black. Defaults false, for gray.
		@param bool (optional): Scaling of the text, either normal or infoPanel size. Defaults to normal
	*/
	Text(const std::string&, sf::Vector2f, int = 0, bool = false, bool = false);

	static void setTexture(const sf::Texture&);		// To not reload texture for every text object

	const sf::Vector2f getPosition() const;

	void edit(const std::string&, bool = false, bool = false);

	std::vector<sf::Sprite> sprites;

private:
	void construct(std::string, bool, bool);

	const static sf::Texture* texture;				// To not reload texture for every text object
	sf::Vector2f position;
	int align;
};