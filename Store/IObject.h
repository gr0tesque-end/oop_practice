#pragma once
#include <string>
#include <ostream>
#include <sstream>

// 5.8
class IObject
{
public:
	// 5.7
	virtual std::stringstream ToString() const = 0;
};

