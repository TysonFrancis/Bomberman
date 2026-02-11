#include "Entity.h"

Entity::Entity(sf::Texture& tex) :
	sprite(tex), alive(true), myFrame(0), myTick(0) {}

sf::Sprite& Entity::getSprite() { return sprite; }

void Entity::setLife(bool life) { alive = life; }
bool Entity::getLife() { return alive; }

void Entity::move(Vector2f move) { sprite.move(move); }
void Entity::setScale(Vector2f scale) { sprite.setScale(scale); }
void Entity::setPosition(Vector2f pos) { sprite.setPosition(pos); }
void Entity::setOrigin(Vector2f origin) { sprite.setOrigin(origin); }

void Entity::setTexture(const sf::IntRect& rect) { sprite.setTextureRect(rect); }

bool Entity::intersects(Entity& other)
{
	if (this->getSprite().getGlobalBounds().
		findIntersection(other.getSprite().getGlobalBounds()))
		return true;
	return false;
}
