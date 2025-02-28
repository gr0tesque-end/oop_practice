#pragma once
#include <string>
#include <ostream>
#include <sstream>

class IObject
{
public:
	IObject* IObjectReference;

	IObject(IObject* const obj) : IObjectReference{ obj } {}

	virtual std::stringstream ToString() const = 0;

	friend std::ostream& operator<<(std::ostream& os, const IObject& l) {
		os << l.ToString().str();
		return os;
	}
};

