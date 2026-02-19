#include "Player.h"

using namespace sf::Keyboard;

Player::Player(sf::Texture& tex,Pod pod[11][29]) : Entity(tex), joyX(0.f), joyY(0.f), speed(4.f) {}

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
		joyX = isKeyPressed(Scan::Right) - isKeyPressed(Scan::Left);
		joyY = isKeyPressed(Scan::Down) - isKeyPressed(Scan::Up);
		
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
			//check if colliding with wall or bomb
			//Current bug:stops player prematurely, needs to wait for actual collision first
			//collide right
			if (joyX > 0&&x<29)
			{
				if(pods[y][x+1].getTile()!=nullptr)
				{
					if (pods[y][x + 1].getTile()->getType() < 3/*add player intersection with pod*/)
						joyX = 0;
				}
			}
			//collide left
			if (joyX < 0&& x > 0)
			{
				if (pods[y][x - 1].getTile() != nullptr)
				{
					if (pods[y][x - 1].getTile()->getType() < 3)
						joyX = 0;
				}
			}
			//collide down
			if (joyY > 0 && y < 11)
			{
				if (pods[y + 1][x].getTile() != nullptr)
				{
					if (pods[y + 1][x].getTile()->getType() < 3)
						joyY = 0;
				}
			}
			//collide up
			if (joyY < 0 && y >0)
			{
				if (pods[y - 1][x].getTile() != nullptr)
				{
					if (pods[y - 1][x].getTile()->getType() < 3)
						joyY = 0;
				}
			}
		}
		//move player and check what pod they are now in
		move({ joyX * speed, joyY * speed });
		int tempX, tempY;
		tempX = getSprite().getPosition().x -8;
		tempY = getSprite().getPosition().y -8;
		x = tempX / 16;
		y = tempY / 16;

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
