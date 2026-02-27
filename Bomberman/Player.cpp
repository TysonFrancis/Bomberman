#include "Player.h"
#include <iostream>

using namespace Constants;
using namespace sf::Keyboard;

Player::Player(const sf::Texture& tex, Pod (&pod)[_rows][_cols]) :
	Entity(tex), pods(pod), speed(4.f), joyX(0.f), joyY(0.f), x(0), y(0) {}

void Player::update()
{
	if (alive)
	{
		// Determine total direction held and move accordingly
		joyX = isKeyPressed(Scan::Right) - isKeyPressed(Scan::Left);
		joyY = isKeyPressed(Scan::Down) - isKeyPressed(Scan::Up);

		// Collision logic
		if (joyX != 0 || joyY != 0)
		{
			// Collide while moving right
			if (joyX > 0 && x < _cols)
			{
				// Check pod directly next for collisions and stop
				if (pods[y][x + 1].getTile() != nullptr)
				{
					if (intersects(pods[y][x + 1]))
					{
						if (pods[y][x + 1].getTile()->getType() < 3)
							joyX = 0;
					}
				}
				// If colliding on diagonals, autocorrect
				if (pods[y + 1][x + 1].getTile() != nullptr)
				{
					if (intersects(pods[y + 1][x + 1]))
					{
						if (pods[y + 1][x + 1].getTile()->getType() < 3)
							joyY = -1;
					}
				}
				if (pods[y - 1][x + 1].getTile() != nullptr)
				{
					if (intersects(pods[y - 1][x + 1]))
					{
						if (pods[y - 1][x + 1].getTile()->getType() < 3)
							joyY = 1;
					}
				}
			}

			// Collide while moving left
			if (joyX < 0 && x > 0)
			{
				//Check next pod if colliding stop
				if (pods[y][x - 1].getTile() != nullptr)
				{
					if (intersects(pods[y][x - 1]))
					{
						if (pods[y][x - 1].getTile()->getType() < 3)
							joyX = 0;
					}
				}
				// If colliding on diagonals, autocorrect
				if (pods[y + 1][x - 1].getTile() != nullptr)
				{
					if (intersects(pods[y + 1][x - 1]))
					{
						if (pods[y + 1][x - 1].getTile()->getType() < 3)
							joyY = -1;
					}
				}
				if (pods[y - 1][x - 1].getTile() != nullptr)
				{
					if (intersects(pods[y - 1][x - 1]))
					{
						if (pods[y - 1][x - 1].getTile()->getType() < 3)
							joyY = 1;
					}
				}
			}

			// Collide while moving down
			if (joyY > 0 && y < _rows)
			{
				// Check the pod directly next to it, if colliding stop
				if (pods[y + 1][x].getTile() != nullptr)
				{
					if (intersects(pods[y + 1][x]))
					{
						if (pods[y + 1][x].getTile()->getType() < 3)
							joyY = 0;
					}
				}
				// If colliding on diagonals, autocorrect
				if (pods[y + 1][x + 1].getTile() != nullptr)
				{
					if (intersects(pods[y + 1][x + 1]))
					{
						if (pods[y + 1][x + 1].getTile()->getType() < 3)
							joyX = -1;
					}
				}
				if (pods[y + 1][x - 1].getTile() != nullptr)
				{
					if (intersects(pods[y + 1][x - 1]))
					{
						if (pods[y + 1][x - 1].getTile()->getType() < 3)
							joyX = 1;
					}
				}
			}

			// Collide while moving up
			if (joyY < 0 && y > 0)
			{
				// Check pod directly next to, if colliding stop
				if (pods[y - 1][x].getTile() != nullptr)
				{
					if (intersects(pods[y - 1][x]))
					{
						if (pods[y - 1][x].getTile()->getType() < 3)
							joyY = 0;
					}
				}
				// If colliding on diagonals, autocorrect
				if (pods[y - 1][x + 1].getTile() != nullptr)
				{
					if (intersects(pods[y - 1][x + 1]))
					{
						if (pods[y - 1][x + 1].getTile()->getType() < 3)
							joyX = -1;
					}
				}
				if (pods[y - 1][x - 1].getTile() != nullptr)
				{
					if (intersects(pods[y - 1][x - 1]))
					{
						if (pods[y - 1][x - 1].getTile()->getType() < 3)
							joyX = 1;
					}
				}
			}
		}

		// Move player and update what pod they're in
		move({ joyX * speed, joyY * speed });

		x = getSprite().getPosition().x / _scaledTile;
		y = getSprite().getPosition().y / _scaledTile;

		// Used to kill player, mainly just for death animation testing
		if (isKeyPressed(Scancode::X))
		{
			alive = false;
			myFrame = myTick = 0;
			std::cout << "Dead";
		}

		// Spawn a bomb
		if (isKeyPressed(Scancode::Z))
			if (pods[y][x].getTile() == nullptr)
				pods[y][x].setTile(new Bomb(true, 2));
	}

	// Animation
	animate();
}

void Player::animate()
{
	myTick++;								// Increment tick every update

	if (myTick % 5 != 0)					// Only update frame every 5 ticks, 60fps -> 12 frames per second
		return;

	if (alive)								// Alive animations
	{
		if (joyX != 0 || joyY != 0)				// Only update texture if moving
		{
			myFrame = (myFrame + 1) % 3;		// Loop through frames for walking animation, 3 frames total

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
			setTexture(sf::IntRect({ myFrame * 16 + xOffset, yOffset }, { _tileSize, _tileSize }));
		}

		return;
	}
											// Death animation
	if (myFrame < 7)						// Keep incrementing frame until finished with death animation
		myFrame++;

	setTexture(sf::IntRect({ myFrame * 16, 32 }, { _tileSize, _tileSize }));
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	return os << "Position: (" << player.x << ", " << player.y << ")\t"
		<< "JoyX: " << player.joyX << "\tJoyY : " << player.joyY << "\n";
}