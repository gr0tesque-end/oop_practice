#pragma once
#include <string>
#include "IObject.h"

class IIdentifiable
	: virtual public IObject
{
protected:
	static int Id;
public:
	IIdentifiable() { ++Id; }

	virtual const int GetId() const { return Id; }
};
