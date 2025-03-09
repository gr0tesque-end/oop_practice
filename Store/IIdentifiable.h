#pragma once
#include <string>
#include "IObject.h"

// 3.5
class IIdentifiable
	: virtual public IObject
{
private:
	static int Id;
protected:
	int id;
public:
	IIdentifiable(int id) : id{ id } {}
	IIdentifiable() : id{ Id } { ++Id; }
	IIdentifiable(IIdentifiable&& idObj) noexcept
		: id{ idObj.id } {}

	virtual const int GetId() const { return id; }
	
	virtual std::stringstream ToString() const override {
			std::stringstream ss;

			ss << "\n{\n\tObject: \"" << "<unidentified>" << "\",\n"
				<< "\tId: \"" << GetId() << "\"\n" << '}';
			return ss;
	}
};

