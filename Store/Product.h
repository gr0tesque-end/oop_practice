#pragma once
#include <string>
#include <iostream>

class Product
{
	float _price;
	std::string _name;
	int _quantity;
public:
	Product()
		: Product{ "" } {
	}
	Product(const std::string& name) 
		: Product{ name, 0 } {
	}

	Product(const std::string& name, float price)
		: Product{ name, price, 0 } {
	}


	Product(const std::string& name, float price, int quantity)
		: _price{ price }, _name{ name }, _quantity{ quantity } {
	}
#pragma region Setters
	void Rename(const std::string& name) {
		_name = name;
	}

	void SetPrice(const float& price) {
		if (price <= 0) { std::cerr << "\"Price\" mustn't be less than 0\n"; return; }
		_price = price;
	} 

	void SetQuantity(const int& quantity) {
		if (quantity < 0) { std::cerr << "\"Quantity\" mustn't be less than 0\n"; return; }
		_quantity = quantity;
	}

	void ChangeQuantity(const int& change) {
		if (change > _quantity) { std::cerr << "\"Change\" mustn't be less than \"Quantity\"\n"; return; }
		_quantity += change;
	}
#pragma endregion

	const std::string& GetName() const { return _name; }

	const int GetPrice() const { return _price; }

	const int GetQuantity() const { return _quantity; }

	bool IsInStock() const { return _quantity > 0; }
};
