#pragma once
#include <string>
#include <ostream>
#include <sstream>
#include <regex>

// 5.8
class IObject
{
public:
	// 5.7
	virtual std::stringstream ToString() const = 0;

	bool operator==(const IObject* obj) const {
		return this->ToString().str() == obj->ToString().str();
	}
};

