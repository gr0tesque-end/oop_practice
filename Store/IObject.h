#pragma once
#include <string>

class IObject
{
protected:
	IObject* IObjectReference = nullptr;
public:
	IObject(IObject* const obj) : IObjectReference{ obj } {}

	virtual std::stringstream ToString() const = 0;
};

