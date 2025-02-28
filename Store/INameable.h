#pragma once
#include <string>
#include "IObject.h"

class INameable
	: virtual public IObject
{
protected:
	std::string Name;
public:
	INameable(const std::string& name) : Name { name } {}

	const std::string& GetName() const { return Name; }
};

