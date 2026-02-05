#pragma once
#include "Animations.h"

class Entity
{
public:
	static Animations animations;

	Animations getAnimation();
};