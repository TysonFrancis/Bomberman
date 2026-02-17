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
		// Determine total direction held and move accordingly
		// vv TODO: ADD COLLISION HERE!!!1! vv
		joyX = isKeyPressed(Scan::Right) - isKeyPressed(Scan::Left);
		joyY = isKeyPressed(Scan::Down) - isKeyPressed(Scan::Up);
		move({ joyX * speed, joyY * speed });

		// Update texture if moving
		if (joyX != 0 || joyY != 0)
		{
			// Texture offsets. Irrelevant to movement or input axes.
			int xOffset = 0, yOffset = 0;

			// Determine the correct texture offset based on input direction
			if (joyY != 0)
			{
				xOffset = 48;
				if (joyY == -1)
					yOffset = 16;
			}
			else if (joyX == 1)
				yOffset = 16;

			// Apply selected texture
			setTexture(sf::IntRect({ myFrame * 16 + xOffset, yOffset}, {16, 16}));
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
