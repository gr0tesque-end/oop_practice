#pragma once

#include <string>
#include <iostream>

class Employee
{
	float _sales;
	std::string _name;

public:
	Employee()
		: Employee{ "" } {
	}
	Employee(const std::string& name)
		: Employee{ name, 0 } {
	}

private:
	Employee(const std::string& name, float sales)
		: _sales{ sales }, _name{ name } {
	}
public: 
#pragma region Setters
	void Rename(const std::string& name) {
		_name = name;
	}

	void ChangeSales(const int& change) {
		if (change <= 0) { std::cerr << "\"Change\" mustn't be less or equal to 0\n"; return; }
		_sales += change;
	}
#pragma endregion

	const std::string& GetName() const { return _name; }

	const int GetSales() const { return _sales; }

	const float CalculateSalary() const { return _sales * 0.3; }
};



