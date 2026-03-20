#include "Entity.h"

using namespace Constants;

Entity::Entity(const Animations& frames, Pod(&pods)[_rows][_cols]) :
	sprite(frames.getEntities()), frames(frames), pods(pods),
	state(State::Living), dir(Facing::None),
	myTick(0), myFrame(0), tileX(0), tileY(0)
{
	setOrigin({ Constants::_halfTile, Constants::_halfTile });
	setScale({ Constants::_scale, Constants::_scale });
}

const sf::Sprite& Entity::getSprite() const		{ return sprite; }
Entity::State Entity::getState() const			{ return state; }

void Entity::move(sf::Vector2f dir)					{ sprite.move(dir); }
void Entity::setScale(sf::Vector2f scale)			{ sprite.setScale(scale); }
void Entity::setPosition(sf::Vector2f pos)			{ sprite.setPosition(pos); }
void Entity::setOrigin(sf::Vector2f origin)			{ sprite.setOrigin(origin); }
void Entity::setTexture(const sf::IntRect& rect)	{ sprite.setTextureRect(rect); }
void Entity::draw(sf::RenderTarget& target,
				sf::RenderStates states) const		{ target.draw(sprite, states); }

bool Entity::intersects(Entity& other) const
{
	if (this->sprite.getGlobalBounds().
		findIntersection(other.sprite.getGlobalBounds()) &&
		this->state == State::Living && other.state == State::Living)
		return true;
	return false;
}

// Takes in coordinates of the pod to check, and creates an sf::FloatRect
// of the area to check for an intersection with the entity's sprite,
// similar to how you could use getGlobalBounds() on an sf::RectangleShape,
// this is just manual. Since there's no shape and just boolean values
// I have to create the rectangle to make the comparison myself.
bool Entity::intersects(float x, float y) const
{
	if (this->getSprite().getGlobalBounds(). // At position (tileX * game tile scale, tileY * game tile scale), with size of game tile scale
		findIntersection(sf::FloatRect(sf::Vector2f(x * _scaledTile, y * _scaledTile), sf::Vector2f(_scaledTile, _scaledTile))))
		return true;
	return false;
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