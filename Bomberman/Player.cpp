#include "Player.h"
#include <iostream>

using namespace Constants;
using namespace sf::Keyboard;

Player::Player(const sf::Texture& tex, Pod (&pods)[_rows][_cols], std::vector<Bomb>& bombs) :
	Entity(tex, pods), bombs(bombs), speed(4.f), joyX(0), joyY(0),
	lives(3), blast(1), maxBombs(1), remote(false)
{
	setTexture(sf::IntRect({ 64, 0 }, { _tileSize, _tileSize }));
	setPosition({ _scaledTile + _halfScaled, _scaledTile + _halfScaled });
}

void Player::update()
{
	if (state == State::Living)
	{
		// Update tile position based on current world position
		tileX = static_cast<int>((sprite.getPosition().x - _halfTile) / _scaledTile);
		tileY = static_cast<int>((sprite.getPosition().y - _halfTile) / _scaledTile);

		// Determine total direction held
		joyX = isKeyPressed(Scan::Right) - isKeyPressed(Scan::Left);
		joyY = isKeyPressed(Scan::Down) - isKeyPressed(Scan::Up);

		moveLogic();

		// Spawn a bomb
		if (isKeyPressed(Scancode::Z))
			if (!pods[tileY][tileX].filled && bombs.size() < maxBombs)
			{
				pods[tileY][tileX].filled = true;
				bombs.push_back(Bomb(getSprite().getTexture(), pods, remote, blast, tileX, tileY));
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

			setTexture(sf::IntRect({ 64, 0 }, { _tileSize, _tileSize }));
			setPosition({ _scaledTile + _halfScaled, _scaledTile + _halfScaled });
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
			setTexture(sf::IntRect({ myFrame * _tileSize + xOffset, yOffset }, { _tileSize, _tileSize }));
		}

		return;
	}
	
	if (state == State::Dying)				// Death animation
	{
		if (myFrame < 7)						// Keep incrementing frame until finished with death animation
		{
			myFrame++;
			setTexture(sf::IntRect({ myFrame * _tileSize, 32 }, { _tileSize, _tileSize }));
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

	if (joyX != 0)						// If moving horizontally,
	{
		if (isObstructed(nextX, tileY))		// If pod in next tile is solid and colliding, stop
			joyX = 0;
		else
		{										// If colliding on diagonals, autocorrect
			if (joyY >= 0 && tileY < _rows - 1 && isObstructed(nextX, tileY + 1))
				joyY = -1;
			else if (joyY <= 0 && tileY > 0 && isObstructed(nextX, tileY - 1))
				joyY = 1;
		}
	}

	if (joyY != 0)						// If moving vertically,
	{
		if (isObstructed(tileX, nextY))		// If pod in next tile is solid and colliding, stop
			joyY = 0;
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

// Adapted collision logic to new
// pod system but not working at all
/*// Right
if (joyX > 0 && x < _cols - 1)
{
	// Check pod directly next for collisions and stop
	if (pods[y][x + 1].filled)
	{
		if (intersects(x + 1, y))
		{
			joyX = 0;
		}
	}
	// If colliding on diagonals, autocorrect
	else
	{
		if (pods[y + 1][x + 1].filled)
		{
			if (intersects(x + 1, y + 1))
			{
				joyY = -1;
			}
		}
		if (pods[y - 1][x + 1].filled)
		{

			if (intersects(x + 1, y - 1))
			{
				joyY = 1;
			}
		}
	}
}

// Left
if (joyX < 0 && x > 0)
{
	// Check pod directly next for collisions and stop
	if (pods[y][x - 1].filled)
	{
		if (intersects(x - 1, y))
		{
			joyX = 0;
		}
	}
	// If colliding on diagonals, autocorrect
	else
	{
		if (pods[y + 1][x - 1].filled)
		{
			if (intersects(x - 1, y + 1))
			{
				joyY = -1;
			}
		}
		if (pods[y - 1][x - 1].filled)
		{

			if (intersects(x - 1, y - 1))
			{
				joyY = 1;
			}
		}
	}
}

// Down
if (joyY > 0 && y < _rows - 1)
{
	// Check pod directly next for collisions and stop
	if (pods[y + 1][x].filled)
	{
		if (intersects(x, y + 1))
		{
			joyY = 0;
		}
	}
	// If colliding on diagonals, autocorrect
	else
	{
		if (pods[y + 1][x + 1].filled)
		{
			if (intersects(x + 1, y + 1))
			{
				joyX = -1;
			}
		}
		if (pods[y + 1][x - 1].filled)
		{

			if (intersects(x - 1, y + 1))
			{
				joyX = 1;
			}
		}
	}
}

// Up
if (joyY < 0 && y > 0)
{
	// Check pod directly next for collisions and stop
	if (pods[y - 1][x].filled)
	{
		if (intersects(x, y - 1))
		{
			joyY = 0;
		}
	}
	// If colliding on diagonals, autocorrect
	else
	{
		if (pods[y - 1][x + 1].filled)
		{
			if (intersects(x + 1, y - 1))
			{
				joyX = -1;
			}
		}
		if (pods[y - 1][x - 1].filled)
		{

			if (intersects(x - 1, y - 1))
			{
				joyX = 1;
			}
		}
	}
}*/

// Original collision logic with pod and tile*
/*// Collision logic
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
				if (pods[y][x + 1].getTile()->isObstruction())
					joyX = 0;
			}
		}
		// If colliding on diagonals, autocorrect
		else{
			if (pods[y + 1][x + 1].getTile() != nullptr)
			{
				if (intersects(pods[y + 1][x + 1]))
				{
					if (pods[y + 1][x + 1].getTile()->isObstruction())
						joyY = -1;
				}
			}
			if (pods[y - 1][x + 1].getTile() != nullptr)
			{

				if (intersects(pods[y - 1][x + 1]))
				{
					if (pods[y - 1][x + 1].getTile()->isObstruction())
						joyY = 1;
				}
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
				if (pods[y][x - 1].getTile()->isObstruction())
					joyX = 0;
			}
		}
		// If colliding on diagonals, autocorrect
		else{
			if (pods[y + 1][x - 1].getTile() != nullptr)
			{
				if (intersects(pods[y + 1][x - 1]))
				{
					if (pods[y + 1][x - 1].getTile()->isObstruction())
						joyY = -1;
				}
			}
			if (pods[y - 1][x - 1].getTile() != nullptr)
			{
				if (intersects(pods[y - 1][x - 1]))
				{
					if (pods[y - 1][x - 1].getTile()->isObstruction())
						joyY = 1;
				}
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
				if (pods[y + 1][x].getTile()->isObstruction())
					joyY = 0;
			}
		}
		// If colliding on diagonals, autocorrect
		else{
			if (pods[y + 1][x + 1].getTile() != nullptr)
			{
				if (intersects(pods[y + 1][x + 1]))
				{
					if (pods[y + 1][x + 1].getTile()->isObstruction())
						joyX = -1;
				}
			}
			if (pods[y + 1][x - 1].getTile() != nullptr)
			{
				if (intersects(pods[y + 1][x - 1]))
				{
					if (pods[y + 1][x - 1].getTile()->isObstruction())
						joyX = 1;
				}
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
				if (pods[y - 1][x].getTile()->isObstruction())
					joyY = 0;
			}
		}
		// If colliding on diagonals, autocorrect
		else{
			if (pods[y - 1][x + 1].getTile() != nullptr)
			{
				if (intersects(pods[y - 1][x + 1]))
				{
					if (pods[y - 1][x + 1].getTile()->isObstruction())
						joyX = -1;
				}
			}
			if (pods[y - 1][x - 1].getTile() != nullptr)
			{
				if (intersects(pods[y - 1][x - 1]))
				{
					if (pods[y - 1][x - 1].getTile()->isObstruction())
						joyX = 1;
				}
			}
		}
	}
}*/