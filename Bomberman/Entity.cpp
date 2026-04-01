#include "Entity.h"

using namespace Constants;

Entity::Entity(const sf::Texture& tex, Pod(&pods)[_rows][_cols]) :
	sprite(tex), pods(pods),
	state(State::Living), dir(Facing::None),
	myTick(0), myFrame(0),
	tileX(0), tileY(0)
{
	setOrigin(_halfTile, _halfTile);
	setScale(_scale, _scale);
}

const sf::Sprite& Entity::getSprite() const		{ return sprite; }
Entity::State Entity::getState() const			{ return state; }
Entity::Facing Entity::getDir() const			{ return dir; }
int Entity::getX() const						{ return tileX; }
int Entity::getY() const						{ return tileY; }

void Entity::move(float x, float y)					{ sprite.move(sf::Vector2f(x, y)); }
void Entity::setScale(float x, float y)				{ sprite.setScale(sf::Vector2f(x, y)); }
void Entity::setPosition(int x, int y)				{ sprite.setPosition(sf::Vector2f(x * _scaledTile + _halfScaled, y * _scaledTile + _halfScaled));
														tileX = x; tileY = y; }
void Entity::setOrigin(float x, float y)			{ sprite.setOrigin(sf::Vector2f(x, y)); }
void Entity::setTexture(int x, int y)				{ sprite.setTextureRect(sf::IntRect(sf::Vector2i(x, y), _tile)); }
void Entity::draw(sf::RenderTarget& target,
				sf::RenderStates states) const		{ target.draw(sprite, states); }

bool Entity::intersects(const Entity& other) const
{
	return this->sprite.getGlobalBounds().findIntersection(other.sprite.getGlobalBounds())
		&& this->state == State::Living && other.state == State::Living;
}

// Takes in coordinates of the pod to check, and creates an sf::FloatRect
// of the area to check for an intersection with the entity's sprite,
// similar to how you could use getGlobalBounds() on an sf::RectangleShape,
// this is just manual. Since there's no shape and just boolean values
// I have to create the rectangle to make the comparison myself.
bool Entity::intersects(int x, int y) const
{
	return this->getSprite().getGlobalBounds().					// At position
		findIntersection(sf::FloatRect(							// (tileX * game tile scale,
		sf::Vector2f(x * _scaledTile, y * _scaledTile),			// tileY * game tile scale),
		sf::Vector2f(_scaledTile, _scaledTile))).has_value();	// with size of game tile scale
}


// *** Public debugging method for derived class use *** //

Entity& Entity::operator=(const Entity& other)
{
	if (this != &other)
	{
		sprite = other.sprite;

		state = other.state;
		dir = other.dir;

		myTick = other.myTick;
		myFrame = other.myFrame;

		tileX = other.tileX;
		tileY = other.tileY;
	}

	return *this;
}