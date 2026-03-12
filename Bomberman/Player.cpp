#include "Player.h"
#include <iostream>

using namespace Constants;
using namespace sf::Keyboard;

Player::Player(const sf::Texture& tex, Pod (&pods)[_rows][_cols],
	std::vector<Bomb>& bombs, std::vector<Explosion>& explosions) :
		Entity(tex, pods), bombs(bombs), explosions(explosions), speed(4.f),
		joyX(0), joyY(0), lives(3), blast(2), maxBombs(3), remote(false)
{
	tileX = tileY = 1;
	setTexture(sf::IntRect({ 64, 0 }, _tile));
	setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
}

void Player::update()
{
	if (state == State::Living)
	{
		// Update tile position based on current world position
		tileX = static_cast<int>((sprite.getPosition().x ) / _scaledTile);
		tileY = static_cast<int>((sprite.getPosition().y ) / _scaledTile);

		// Determine total direction held
		joyX = isKeyPressed(Scan::Right) - isKeyPressed(Scan::Left);
		joyY = isKeyPressed(Scan::Down) - isKeyPressed(Scan::Up);

		moveLogic();

		// Spawn a bomb
		if (isKeyPressed(Scancode::Z))
			if (!pods[tileY][tileX].filled && bombs.size() < maxBombs)
			{
				pods[tileY][tileX].filled = true;
				pods[tileY][tileX].isBomb = true;
				bombs.push_back(Bomb(sprite.getTexture(), pods, explosions, remote, blast, tileX, tileY));
			}
	}

	// Handles respwawning if player dies but has lives left,
	// otherwise waits for animation to finish and fully die
	if(state == State::Dead)
	{
		//std::cout << "Died...\tlives: " << lives - 1 << "\n";

		if(lives - 1 > 0)
		{
			lives--;
			state = State::Living;

			setTexture(sf::IntRect({ 64, 0 }, _tile));
			setPosition(sf::Vector2f(tileX * _scaledTile + _halfScaled, tileY * _scaledTile + _halfScaled));
		}
	}

	// Animation
	animate();
}

void Player::animate()
{
	myTick++;								// Increment tick every update

	if (myTick % 5 != 0)					// Only update frame every 5 ticks, 60fps -> 12 frames per second
		return;

	if (state == State::Living)								// Alive animations
	{
		if (joyX != 0 || joyY != 0)				// Only update texture if moving
		{
			myFrame = (myFrame + 1) % _moveFrames;		// Loop through frames for walking animation

			// Texture offsets. Irrelevant to movement or input axes
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
			setTexture(sf::IntRect({ myFrame * _tileSize + xOffset, yOffset }, _tile));
		}

		return;
	}
	
	if (state == State::Dying)				// Death animation
	{
		if (myFrame < 7)						// Keep incrementing frame until finished with death animation
		{
			myFrame++;
			setTexture(sf::IntRect({ myFrame * _tileSize, 32 }, _tile));
		}

		if (myFrame >= 7)						// Once animation is finished, fully die
			state = State::Dead;
	}
}

void Player::die()
{
	if (state != State::Living)
		return;

	state = State::Dying;
	myFrame = myTick = 0;
	tileX = tileY = 1;
}


// *** Private helper methods *** //

// Gets stuck on corners real bad if walking into them,
// but works decently well otherwise compared to old system
//
// This switch is good to separate pod into not rendering textures
// and messing with tile types and what not... makes bomb and softwall
// animations exponentially easier but it seems the collision isn't as sound
// 
// Also need to work on centering player in square when
// moving, probably the cause of a lot of the jittering					- Dylan
void Player::moveLogic()
{
	int nextX = tileX + joyX;			// Calculate next tile position based on input to avoid 
	int nextY = tileY + joyY;			// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.
	
	if (joyX > 0)			//If moving right, only check if collision if person is on the right half of the tile
	{
		if (pods[tileY][nextX].filled)
		{
			if ((tileX * _scaledTile + _halfScaled) <= getSprite().getPosition().x)
			{
				joyX = 0;
			}
		}
		else
		{										// If colliding on diagonals, autocorrect
			if (joyY >= 0 && tileY < _rows - 1 && isObstructed(nextX, tileY + 1))
				joyY = -1;
			else if (joyY <= 0 && tileY > 0 && isObstructed(nextX, tileY - 1))
				joyY = 1;
		}
	}

	if (joyX < 0)			//If moving left, only check if collision if person is on the right half of the tile
	{
		if (pods[tileY][nextX].filled)
		{
			if ((tileX * _scaledTile + _halfScaled) >= getSprite().getPosition().x)
			{
				joyX = 0;
			}
		}
		else
		{										// If colliding on diagonals, autocorrect
			if (joyY >= 0 && tileY < _rows - 1 && isObstructed(nextX, tileY + 1))
				joyY = -1;
			else if (joyY <= 0 && tileY > 0 && isObstructed(nextX, tileY - 1))
				joyY = 1;
		}
	}
	if (joyY > 0)			//If moving down, only check if collision if person is on the lower half of the tile
	{
		if (pods[nextY][tileX].filled)
		{
			if ((tileY * _scaledTile + _halfScaled) <= getSprite().getPosition().y)
			{
				joyY = 0;
			}
		}
		else
		{										// If colliding on diagonals, autocorrect
			if (joyX >= 0 && tileX < _cols - 1 && isObstructed(tileX + 1, nextY))
				joyX = -1;
			else if (joyX <= 0 && tileX > 0 && isObstructed(tileX - 1, nextY))
				joyX = 1;
		}
	}
	if (joyY < 0)			//If moving up, only check if collision if person is on the upper half of the tile
	{
		if (pods[nextY][tileX].filled)
		{
			if ((tileY * _scaledTile + _halfScaled) >= getSprite().getPosition().y)
			{
				joyY = 0;
			}
		}
		else
		{										// If colliding on diagonals, autocorrect
			if (joyX >= 0 && tileX < _cols - 1 && isObstructed(tileX + 1, nextY))
				joyX = -1;
			else if (joyX <= 0 && tileX > 0 && isObstructed(tileX - 1, nextY))
				joyX = 1;
		}
	}

	move({ joyX * speed, joyY * speed });		// Move based on input and speed
}

bool Player::isObstructed(int checkX, int checkY)
{
	if (checkX < 0 || checkX >= _cols ||		// Out of bounds, treat as solid
		checkY < 0 || checkY >= _rows)
		return true;
	
	// return if pod in question is solid and colliding
	return pods[checkY][checkX].filled && intersects(checkX, checkY);
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	return os << "Position: (" << player.tileX << ", " << player.tileY << ")\t"
		<< "JoyX: " << player.joyX << "\tJoyY : " << player.joyY << "\n";
}

Player& Player::operator=(const Player& other)
{
	if (this != &other)
		Entity::operator=(other);
	return *this;
}