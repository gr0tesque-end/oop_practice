#pragma once
#include <string>
#include "IObject.h"

class IIdentifiable
	: virtual public IObject
{
protected:
	static int Id;
	int id;
public:
	IIdentifiable() : id{ Id } { ++Id; }

	virtual const int GetId() const { return id; }
};
