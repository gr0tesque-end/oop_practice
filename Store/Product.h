#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "Account.h"
#include "IIdentifiable.h"

class Product
	: public INameable, public IIdentifiable
{
	float _price;
	int _quantity;
public:
	const static std::string Type;

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
		: _price{ price }, IIdentifiable(), INameable(name), _quantity{ quantity } {
	}
	Product(int id, const std::string& name, float price, int quantity)
		: _price{ price }, INameable(name), _quantity{ quantity }, IIdentifiable{ id } {
	}

	Product(const std::string& id, const std::string& name, const std::string& price, const std::string& quantity)
		: Product{ atoi(id.c_str()), name, static_cast<float>(atof(price.c_str())), atoi(quantity.c_str()) } {
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
		_quantity += change;
	}
#pragma endregion

	const float GetPrice() const { return _price; }

	const int GetQuantity() const { return _quantity; }

	bool IsInStock() const { return _quantity > 0; }

	virtual const int GetId() const override { return id; }

	// 3.6
	Product& operator++(int) {
		ChangeQuantity(1);
		return *this;
	}

	Product& operator++() {
		ChangeQuantity(1);
		return *this;
	}

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss << "\n{\n\t\"Object\": \"" << "Product" << "\",\n"
			<< "\t\"Id\": \"" << GetId() << "\",\n"
			<< "\t\"Name\": \"" << Name << "\",\n"
			<< "\t\"Price\": \"" << _price << "\",\n"
			<< "\t\"Quantity\": \"" << _quantity << "\"\n" << "},";
		return ss;
	};

	friend class Store;
};
const std::string Product::Type{ "Product" };