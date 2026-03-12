#pragma once

class Pod
{
public:
	Pod() : filled(true), isSoft(false), isHard(false), isBomb(false) {}

	bool filled;
	bool isSoft;
	bool isHard;
	bool isBomb;
};