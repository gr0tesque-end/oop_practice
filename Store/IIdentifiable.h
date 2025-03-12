#pragma once
#include <string>
#include "IObject.h"

// 3.5
class IIdentifiable
	: virtual public IObject
{
protected:
	int id;
public:
	IIdentifiable(int id) : id{ id } {}
	IIdentifiable() : id{ -1 } {}
	IIdentifiable(IIdentifiable&& idObj) noexcept
		: id{ idObj.id } {
		idObj.id = -1;
	}

	virtual const int GetId() const { return id; }

	friend class Db;
};

