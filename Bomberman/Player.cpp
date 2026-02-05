#include "Player.h"

using namespace sf::Keyboard;


Player::Player()
{
	tempDisplay = sf::RectangleShape({ 25, 25 });
	tempDisplay.setFillColor(sf::Color::Blue);
	joyX = 0;
	joyY = 0;
}

Player::~Player() {}


void Player::tick(sf::RenderWindow& window)
{
	joyX = isKeyPressed(Scan::D) - isKeyPressed(Scan::A);
	joyY = isKeyPressed(Scan::S) - isKeyPressed(Scan::W); //might need reversed idk can't test

	window.draw(tempDisplay);
}