#include "Pod.h"

// ********** TYSON LOGIC STUFF ********** START ********** //
Pod::Pod(sf::RectangleShape shapes, int ex, int why)
{
	shape = shapes;
	filled = true;
	x = ex;
	y = why;

}

void Pod::fill()
{
	filled = true;
	shape.setFillColor(sf::Color(0, 125, 125));
}
// ********** END ********** //