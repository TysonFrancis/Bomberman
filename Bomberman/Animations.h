#pragma once
#include <SFML/Graphics.hpp>

/*
	Texture and image handler class, loads in all items and sets
	them in appropriate containers. Has getter methods for each
	separate spritesheet as well as game console icon.

	Entity frames are 16 x 16 unless otherwise noted:
tional

		Player at (0, 0), death at (0, 32),

		bomb at (0, 48), blocks at (48, 48), exit door at (176, 48),
		explosions at (0, 64)	-	-	-	-	-	-	-	-	-	-	80 x 80,
		powerups + secrets at (0, 224),

		balloon enemy at (0, 240), death at (96, 240),
		ice cream enemy at (0, 256), death at (96, 256),
		barrell enemy at (0, 272), death at (96, 272),
		circle enemy at (0, 288), death at (96, 288),
		puddle enemy at (0, 304), death at (96, 304),
		ghost enemy at (0, 320), death at (96, 320),
		spikey circle enemy at (0, 336), death at (96, 336),
		coin enemy at (0, 352), death at (96, 352),

		salmon colored deaths continued at (112, 240),
		purple colored deaths continued at (112, 272),
		blue colored deathed contined at (112, 288),

		points at (112, 336)	-	-	-	-	-	-	-	-	-	-	16 x 8,

		secret player *ONLY LEFT* at (176, 352)

	Title frames are 256 x 240, and also contains letters and numbers that are 8 x 8:

		Title 1 at (0, 0),
		Title 2 at (256, 0),

		Grey outlined numbers at (0, 248),
		letters at (0, 256),

		Black outlined numbers at (0, 264),
		letters at (0, 272)

	Background frame is 496 x 208, and also contains blocks that are 16 x 16:

		Background at (0, 0),
		blocks at (16, 208)

	Misc frames are 16 x 16 and are repeats of powerups + secrets from entities file, but a darker shade:

		powerups at (0, 0),
		screts at(0, 16)

*/

class Animations
{
public:
	Animations();

	sf::Texture& getEntities();
	sf::Texture& getBackground();
	sf::Texture& getMisc();
	sf::Texture& getTitle();
	sf::Image& getIcon();

private:
	sf::Texture entities;
	sf::Texture background;
	sf::Texture misc;
	sf::Texture title;
	sf::Image icon;
};