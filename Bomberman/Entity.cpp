#include "Entity.h"

Entity::Entity(const sf::Texture& tex) :
	sprite(tex), state(State::Living), myFrame(0), myTick(0) {}

const sf::Sprite& Entity::getSprite() const { return sprite; }
Entity::State Entity::getState() const { return state; }

void Entity::move(sf::Vector2f dir) { sprite.move(dir); }
void Entity::setScale(sf::Vector2f scale) { sprite.setScale(scale); }
void Entity::setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }
void Entity::setOrigin(sf::Vector2f origin) { sprite.setOrigin(origin); }
void Entity::setTexture(const sf::IntRect& rect) { sprite.setTextureRect(rect); }

bool Entity::intersects(Entity& other) const
{
	if (this->sprite.getGlobalBounds().
		findIntersection(other.sprite.getGlobalBounds()) &&
		this->state == State::Living && other.state == State::Living)
		return true;
	return false;
}

bool Entity::intersects(Pod& pod) const
{
	if (this->getSprite().getGlobalBounds().
		findIntersection(pod.getShape().getGlobalBounds()))
		return true;
	return false;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(sprite, states); }

Entity& Entity::operator=(const Entity& other)
{
	if (this == &other)
		return *this;

	sprite = other.sprite;
	state = other.state;
	myTick = other.myTick;
	myFrame = other.myFrame;

	return *this;
}