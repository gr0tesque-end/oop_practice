#pragma once
#include <string>
#include <ostream>
#include <sstream>

const int AuthorityEmployeeThreshhold{ 100 };

// 5.8
class IObject
{
public:
	// 5.7
	virtual std::stringstream ToString() const = 0;
	virtual ~IObject() {}

	bool operator==(const IObject* obj) const {
		return this->ToString().str() == obj->ToString().str();
	}
};

