#pragma once
#include <SFML/Graphics.hpp>

class Bomb
{
public:
	Bomb(float, float, bool, int[10][10], int); //Temporary array
	~Bomb();
	void tick(int[10][10]);
	void explode(int[10][10]);

private:
	int ticks;
	int distance;
	int xPosition;
	int yPosition;
};