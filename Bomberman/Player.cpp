#include "Player.h"

using namespace sf::Keyboard;

Player::Player(sf::Texture& tex) : Entity(tex), joyX(0.f), joyY(0.f), speed(4.f) {}

void Player::update()
{
	// *** Animation timing ***
	myTick++;									// Increment tick counter every game frame

	if (alive)								    // If alive,
	{
		if (myTick % 5 == 0)				        // Update frame every 5 ticks, 60fps -> 12 frames per second
			myFrame = (myFrame + 1) % 3;	        // Loop through frames for waling animation, 3 frames total
	}

	else                                        // Else,
	{
		if (myTick % 5 == 0)
			myFrame++;								// Update frame every 5 ticks, but don't loop
		if (myFrame >= 7)
			myFrame = 7;                            // Freeze on empty frame once finished
	}

	// *** Sprite movement and texture updates ***
	if (alive)									// Living animations + movement
	{
		if (isKeyPressed(Scancode::Up))
		{
			setTexture(sf::IntRect({ myFrame * 16 + 48, 16 }, { 16, 16 }));
			move({ 0, -speed, });
		}

		else if (isKeyPressed(Scancode::Down))
		{
			setTexture(sf::IntRect({ myFrame * 16 + 48, 0 }, { 16, 16 }));
			move({ 0, speed, });
		}

		if (isKeyPressed(Scancode::Left))
		{
			setTexture(sf::IntRect({ myFrame * 16, 0 }, { 16, 16 }));
			move({ -speed, 0 });
		}

		else if (isKeyPressed(Scancode::Right))
		{
			setTexture(sf::IntRect({ myFrame * 16, 16 }, { 16, 16 }));
			move({ speed, 0 });
		}

		// Used to kill player, mainly just for death animation testing
		if (isKeyPressed(Scancode::X))
		{
			alive = false;
			myFrame = myTick = 0;
		}
	}

	else										// Death animation
		setTexture(sf::IntRect({ myFrame * 16, 32 }, { 16, 16 }));
}

//joyX = isKeyPressed(Scan::D) - isKeyPressed(Scan::A);
//joyY = isKeyPressed(Scan::S) - isKeyPressed(Scan::W); //might need reversed idk can't test