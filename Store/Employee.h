#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "INameable.h"
#include "IIdentifiable.h"

class Employee
	: public INameable, public IIdentifiable
{
	float _sales;
public:
	Employee(const std::string& name = "")
		: INameable{ name }, IIdentifiable{ }, IObject{ this }, _sales{ 0.0f } {
	}

	Employee& ChangeSales(const float& change) {
		if (change <= 0) { std::cerr << "\"Change\" mustn't be less or equal to 0\n"; return *this; }
		_sales += change;
		return *this;
	}

	virtual const int GetId() const override { return Id * 100; }

	const float GetSales() const { return _sales; }

	const float CalculateSalary() const { return _sales * 0.3f; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss  << "{\n\tObject: \"" << "Employee" << "\",\n"
			<< "\tId: \"" << GetId() << "\",\n"
			<< "\tName: \"" << Name << "\",\n"
			<< "\tSales: \"" << _sales << "\",\n" << "}";
		return ss;
	};
};



