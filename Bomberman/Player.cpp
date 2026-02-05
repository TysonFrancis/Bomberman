#include "Player.h"

using namespace sf::Keyboard;


Player::Player()
{
	sf::CircleShape player(20);

	joyX = 0;
	joyY = 0;
}

Player::~Player() {}


void Player::tick()
{
	joyX = isKeyPressed(Scan::D) - isKeyPressed(Scan::A);
	joyY = isKeyPressed(Scan::S) - isKeyPressed(Scan::W); //might need reversed idk can't test
}