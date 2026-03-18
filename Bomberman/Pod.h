#pragma once

class Pod
{
public:
	Pod() : isFilled(false), isHard(false),
		isSoft(false), isBomb(false), isExit(false) {}

	bool isFilled, isHard, isSoft, isBomb, isExit;
};