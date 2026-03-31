#include "Pod.h"
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Pod& pod)
{
	return out << "Pod: " << std::boolalpha
		<< "\tfilled: " << pod.isFilled
		//<< "\thard: " << pod.isHard
		//<< "\tsoft: " << pod.isSoft
		<< "\tbomb: " << pod.isBomb
		//<< "\texit: " << pod.isExit
		<< "\n";
}