#include "Player.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Pod.h"
#include <iostream>
#include <cmath>

using namespace Constants;
using namespace sf::Keyboard;
using std::cout, std::endl;

Player::Player(const sf::Texture& tex, Pod (&pods)[_rows][_cols],
	std::vector<Bomb>& bombs, std::vector<Explosion>& explosions) :
		Entity(tex, pods), bombs(bombs), explosions(explosions),
		speed(_playerSpeed * _speedScale),
		joyX(0), joyY(0), lives(3),
		blast(1), maxBombs(1), wait(0), remote(false),
		isFireShield(false), isInvincible(false),
		wallPhase(false), bombPhase(false),
		justDied(false)
{
	setTexture(64, 0);
	setPosition(1, 1);
}

void Player::update()
{
	if (state == State::Living)
	{
		// Update tile position based on current world position
		tileX = static_cast<int>((sprite.getPosition().x) / _scaledTile);
		tileY = static_cast<int>((sprite.getPosition().y) / _scaledTile);

		// Determine total direction held
		joyX = 0;
		joyY = 0;

		joyX += (isKeyPressed(Scan::Right) - isKeyPressed(Scan::Left));
		joyY += (isKeyPressed(Scan::Down) - isKeyPressed(Scan::Up));

		joyX += static_cast<int>(std::round(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X)));
		joyY += static_cast<int>(std::round(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y)));

		moveLogic();

		// Spawn a bomb
		if (isKeyPressed(Scancode::Z))
			if (!pods[tileY][tileX].isFilled && static_cast<int>(bombs.size()) < maxBombs && !pods[tileY][tileX].isExit)
			{
				pods[tileY][tileX].isFilled = true;
				pods[tileY][tileX].isHard = true;
				pods[tileY][tileX].isBomb = true;
				bombs.emplace_back(sprite.getTexture(), pods, explosions, remote, blast, tileX, tileY);
			}

		//Remote Detonation
		if (wait > 0)
			wait--;
		if ((isKeyPressed(Scancode::X)) && remote && bombs.size() > 0 && wait == 0)
		{
			bombs[0].die();
			wait = 10;
		}
	}

	animate();
}

void Player::animate()
{
	tick++;

	if (tick % _playerTickSpeed != 0)			// Leave method if not time to update frame yet
		return;

	if (state == State::Living)					// Alive animations
	{
		if (joyX != 0 || joyY != 0)					// Only update texture if moving
		{
			frame = (frame + 1) % _moveFrames;			// Loop through frames for walking animation

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
			setTexture(frame * _tileSize + xOffset, yOffset);
		}

		return;
	}

	if (state == State::Dying)					// Death animation
	{
		if (frame < _playerDeathFrames)				// Keep incrementing frame until finished with death animation
		{
			frame++;
			setTexture(frame * _tileSize, _playerDeathY);
		}

		if (frame >= _playerDeathFrames)		// Once animation is finished, fully die
			state = State::Dead;
	}
}

void Player::die()
{
	if (state != State::Living)
		return;

	state = State::Dying;
	frame = tick = 0;
	lives--;

	justDied = true;
}

void Player::reset()
{
	state = State::Living;

	setTexture(64, 0);
	setPosition(1, 1);

	frame = tick = 0;
	tileX = tileY = 1;
	joyX = joyY = 0;
	justDied = false;
	isInvincible = false;
}

void Player::gameReset()
{
	reset();

	speed = _playerSpeed;
	lives = 3;
	blast = 1;
	maxBombs = 1;
	wait = 0;
	remote = false;
	isFireShield = false;
	wallPhase = false;
	bombPhase = false;
}

void Player::extraBomb()				{ if (maxBombs < _bombCountMax)
											maxBombs++; }
void Player::extraRange()				{ if (blast < _bombRangeMax)
											blast++; }
void Player::giveRemote()				{ remote = true; }
void Player::giveSkate()				{ if (speed < _playerSpeed * _speedScale * 1.5f)
											speed *= (1.5f * _speedScale); }
void Player::phaseWalls()				{ wallPhase = true; }
void Player::phaseBombs()				{ bombPhase = true; }
void Player::shieldFire()				{ isFireShield = true; }
void Player::invincible()				{ isInvincible = true; }

bool Player::hasFireShield() const		{ return isFireShield; }
bool Player::hasInvinciblity() const	{ return isInvincible; }
void Player::removeInvincibility()		{ isInvincible = false; }

int Player::getLives() const			{ return lives; }
void Player::addLife()					{ lives++; }

bool Player::isOnExit() const			{ return pods[tileY][tileX].isExit; }
bool Player::isDead() const				{ return state == State::Dead; }
bool Player::hasJustDied()
{
	if (justDied)
	{
		justDied = false;
		return true;
	}

	return false;
}


// *** Private helper methods *** //

void Player::moveLogic()
{
	int nextX = tileX + joyX;					// Calculate next tile position based on input to avoid 
	int nextY = tileY + joyY;						// repeated if blocks of y + 1, y - 1, x + 1, x - 1, etc.

	const Pod& podX = pods[tileY][nextX];		// Make pod references for less written code
	const Pod& podY = pods[nextY][tileX];

	if (joyX > 0)			//If moving right, only check if collision if person is on the right half of the tile
	{
		if (!podX.isFilled || (podX.isBomb && bombPhase) || (podX.isSoft && wallPhase))
		{
			if (joyY >= 0 && tileY < _rows - 1 && isObstructed(nextX, tileY + 1))
				joyY = -1;
			else if (joyY <= 0 && tileY > 0 && isObstructed(nextX, tileY - 1))
				joyY = 1;
		}
		else
			if ((tileX * _scaledTile + _halfScaled) <= getSprite().getPosition().x)
				joyX = 0;
	}

	if (joyX < 0)			//If moving left, only check if collision if person is on the right half of the tile
	{
		if (!podX.isFilled || (podX.isBomb && bombPhase) || (podX.isSoft && wallPhase))
		{
			if (joyY >= 0 && tileY < _rows - 1 && isObstructed(nextX, tileY + 1))
				joyY = -1;
			else if (joyY <= 0 && tileY > 0 && isObstructed(nextX, tileY - 1))
				joyY = 1;
		}
		else
			if ((tileX * _scaledTile + _halfScaled) >= getSprite().getPosition().x)
				joyX = 0;
	}

	if (joyY > 0)			//If moving down, only check if collision if person is on the lower half of the tile
	{
		if (!podY.isFilled || (podY.isBomb && bombPhase) || (podY.isSoft && wallPhase))
		{
			if (joyX >= 0 && tileX < _cols - 1 && isObstructed(tileX + 1, nextY))
				joyX = -1;
			else if (joyX <= 0 && tileX > 0 && isObstructed(tileX - 1, nextY))
				joyX = 1;
		}
		else
			if ((tileY * _scaledTile + _halfScaled) <= getSprite().getPosition().y)
				joyY = 0;
	}

	if (joyY < 0)			//If moving up, only check if collision if person is on the upper half of the tile
	{
		if (!podY.isFilled || (podY.isBomb && bombPhase) || (podY.isSoft && wallPhase))
		{
			if (joyX >= 0 && tileX < _cols - 1 && isObstructed(tileX + 1, nextY))
				joyX = -1;
			else if (joyX <= 0 && tileX > 0 && isObstructed(tileX - 1, nextY))
				joyX = 1;
		}
		else
			if ((tileY * _scaledTile + _halfScaled) >= getSprite().getPosition().y)
				joyY = 0;
	}

	move(joyX * speed, joyY * speed);		// Move based on input and speed
}

bool Player::isObstructed(int checkX, int checkY)
{
	if (checkX < 0 || checkX >= _cols ||		// Out of bounds, treat as solid
		checkY < 0 || checkY >= _rows)
		return true;

	const Pod& pod = pods[checkY][checkX];

	if ((pod.isBomb && bombPhase) || (pod.isSoft && wallPhase))
		return false;

	// return if pod in question is solid and colliding
	return pod.isFilled && intersects(checkX, checkY);
}


// *** Public debugging methods *** //

std::ostream& operator<<(std::ostream& out, const Player& player)
{
	out /*<< "Position: (" << player.tileX << ", " << player.tileY << ')'
		<< "\tJoyX: " << player.joyX << "\tJoyY : " << player.joyY*/
		<< "\tTick: " << player.tick << "\tFrame: " << player.frame
		<< "\tLives: " << player.lives;
	out << "\n";

	return out;
}

Player& Player::operator=(const Player& other)
{
	if (this != &other)
	{
		Entity::operator=(other);

		speed = other.speed;

		joyX = other.joyX;
		joyY = other.joyY;

		lives = other.lives;

		blast = other.blast;
		maxBombs = other.maxBombs;
		remote = other.remote;
		wait = other.wait;
	}

	return *this;
}