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
		: Account(name, pass, Authority), _sales{ sales }, IIdentifiable{ id } {
	}

	Employee(const std::string& id,
		const std::string& name,
		const std::string& sales,
		const std::string& Authority,
		const std::string& pass)
		: Employee{ atoi(id.c_str()), name, static_cast<float>(atof(sales.c_str())), atoi(Authority.c_str()), pass } {
	}

	Employee(const Employee& emp)
		: Account{ emp }, _sales{ emp._sales } { }

	Employee& ChangeSales(const float& change) {
		if (change <= 0) { std::cerr << "\"Change\" mustn't be less or equal to 0\n"; return *this; }
		_sales += change;
		return *this;
	}

	virtual const int GetId() const override { return id; }

	const float GetSales() const { return _sales; }

	const float CalculateSalary() const { return _sales * 0.3f; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss  << "\n{\n\t\"Object\": \"" << "Employee" << "\",\n"
			<< "\t\"Id\": \"" << GetId() << "\",\n"
			<< "\t\"Name\": \"" << Name << "\",\n"
			<< "\t\"Sales\": \"" << _sales << "\",\n"
			<< "\t\"Authority\": \"" << this->Authority << "\",\n"
			<< "\t\"Pass\": \"" << this->password << "\"\n" << "},";
		return ss;
	};
};



