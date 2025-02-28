#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "INameable.h"
#include "IIdentifiable.h"

class Product
	: public INameable, IIdentifiable
{
	float _price;
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
		: _price{ price }, IObject{ this }, INameable { name }, _quantity{ } {
	}
#pragma region Setters
	void Rename(const std::string& name) {
		Name = name;
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

	const float GetPrice() const { return _price; }

	const int GetQuantity() const { return _quantity; }

	bool IsInStock() const { return _quantity > 0; }

	virtual const int GetId() const override { return Id * 10000; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss << "{\n\tObject: \"" << "Product" << "\",\n"
			<< "\tId: \"" << Id << "\",\n"
			<< "\tName: \"" << Name << "\",\n"
			<< "\tPrice: \"" << _price << "\",\n"
			<< "\tQuantity: \"" << _quantity << "\",\n" << "}";
		return ss;
	};
};
