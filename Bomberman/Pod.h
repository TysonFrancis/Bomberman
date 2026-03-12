#pragma once

class Pod
{
public:
	Pod() : filled(true), isSoft(false), isHard(false), isExplosion(false) {}

	bool filled;
	bool isSoft;
	bool isHard;
	bool isExplosion;
};