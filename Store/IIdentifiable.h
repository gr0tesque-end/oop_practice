#pragma once
#include <string>
#include "IObject.h"

class IIdentifiable
	: virtual public IObject
{
private:
	static int Id;
protected:
	IIdentifiable(int id) : id{ id } {}
	int id;
public:
	IIdentifiable() : id{ Id } { ++Id; }
	IIdentifiable(IIdentifiable&& idObj) noexcept
		: id{ idObj.id } {}

	virtual const int GetId() const { return id; }
};
