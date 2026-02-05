#include "Pod.h"


Pod::Pod(sf::RectangleShape input)
{
	shape = input;
}


Pod::~Pod() {}


void Pod::fill()
{
	if(filled==0)
	shape.setFillColor(sf::Color::White);
}