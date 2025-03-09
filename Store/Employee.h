#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "Account.h"
#include "IIdentifiable.h"

class Employee
	: public Account, public IIdentifiable
{
	float _sales;
public:
	Employee(int id, const std::string& name, float sales, int Authority, const std::string& pass)
		: Account(name, pass, Authority), _sales{ sales } {
		this->id = id;
	}

	Employee& ChangeSales(const float& change) {
		if (change <= 0) { std::cerr << "\"Change\" mustn't be less or equal to 0\n"; return *this; }
		_sales += change;
		return *this;
	}

	virtual const int GetId() const override { return 100 + id; }

	const float GetSales() const { return _sales; }

	const float CalculateSalary() const { return _sales * 0.3f; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss  << "\n{\n\tObject: \"" << "Employee" << "\",\n"
			<< "\tId: \"" << GetId() << "\",\n"
			<< "\tName: \"" << Name << "\",\n"
			<< "\tSales: \"" << _sales << "\",\n"
			<< "\tAuthority: \"" << this->Authority << "\",\n"
			<< "\tPass: \"" << this->password << "\"\n" << "}";
		return ss;
	};
};



