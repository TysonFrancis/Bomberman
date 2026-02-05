#include "Animations.h"

void Animations::draw(sf::RenderWindow& window) { window.draw(sprite); }
void Animations::setPosition(float x, float y) { sprite.setPosition({ x, y }); }
void Animations::setScale(float x, float y) { sprite.setScale({ x, y }); }