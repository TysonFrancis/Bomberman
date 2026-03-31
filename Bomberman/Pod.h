#pragma once
#include <iosfwd> // To allow for friend ostream operator<< without including iostream in this header

class Pod
{
public:
	Pod() : isFilled(false), isHard(false),
		isSoft(false), isBomb(false), isExit(false) {}

	bool isFilled, isHard, isSoft, isBomb, isExit;

	friend std::ostream& operator<<(std::ostream&, const Pod&);
};