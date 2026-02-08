#include "Entity.h"

Entity::Entity(sf::Texture& tex) :
	sprite(tex), alive(true), myFrame(0), myTick(0) {}

sf::Sprite& Entity::getSprite() { return sprite; }

void Entity::setLife(bool life) { alive = life; }
bool Entity::getLife() { return alive; }