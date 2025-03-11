#pragma once
#include "Authenticator.h"

class Store
{
	Db* db = Db::GetInstance();
public:
	
	bool Purchase(const std::vector<std::string>& data /* Id & Quantity */) {
		int id = atoi(data[0].c_str());
		

		return true;
	}

	std::stringstream Catalog() const {
		std::stringstream res;
		for (auto p: db->Products)
		{
			res << p->ToString().str() << std::endl;
		}
		return res;
	}

	friend class CLI;
};

