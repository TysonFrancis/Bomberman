#include "Entity.h"

Entity::Entity(const sf::Texture& tex) :
	sprite(tex), alive(true), myFrame(0), myTick(0) {}

const sf::Sprite& Entity::getSprite() const { return sprite; }
bool Entity::isAlive() const { return alive; }

void Entity::setLife(bool life) { alive = life; }

void Entity::move(sf::Vector2f dir) { sprite.move(dir); }
void Entity::setScale(sf::Vector2f scale) { sprite.setScale(scale); }
void Entity::setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }
void Entity::setOrigin(sf::Vector2f origin) { sprite.setOrigin(origin); }
void Entity::setTexture(const sf::IntRect& rect) { sprite.setTextureRect(rect); }

bool Entity::intersects(Entity& other) const
{
	if (this->getSprite().getGlobalBounds().
		findIntersection(other.getSprite().getGlobalBounds()))
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