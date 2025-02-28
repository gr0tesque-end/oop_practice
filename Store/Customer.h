#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "INameable.h"
#include "IIdentifiable.h"
#include "Misc.h"

class Customer
	: public INameable, IIdentifiable
{
	const short PURCHASE_HISTORY_MAX_DEPTH = 10;

	float _balance;

	float* _PurchaseHistory = new float[PURCHASE_HISTORY_MAX_DEPTH];
	int _index = 0;

	void ReInitHistory() {
		if (_PurchaseHistory) delete[] _PurchaseHistory;
		_PurchaseHistory = new float[PURCHASE_HISTORY_MAX_DEPTH];
	}
public:
	Customer(const std::string& name = "", float balance = 0)
		: INameable{ name }, IObject{ this }, _balance{ balance } {
	}

	~Customer() {
		if (_PurchaseHistory) delete[] _PurchaseHistory;
		std::cout << "~Customer\n";
	}

	bool Buy(const float& price) {
		if (price > _balance) { std::cerr << "Insufficient funds\n"; return false; }
		_balance -= price;
	
		if (_index == PURCHASE_HISTORY_MAX_DEPTH) 
		{
			ReInitHistory();
			_index = 0;
		}

		_PurchaseHistory[_index++] = price;

		return true;
	}

	virtual const int GetId() const override { return id * 1000; }

	const int GetIndex() const { return _index; }
	const float* const GetHistory() const { return _PurchaseHistory; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss << "{\n\tObject: \"" << "Customer" << "\",\n"
			<< "\tId: \"" << Id << "\",\n"
			<< "\tName: \"" << Name << "\",\n"
			<< "\tBalance: \"" << _balance << "\",\n" 
			<< "\tPurchaseHistory: \"[ " << Misc::ArrToStr<float*>(_PurchaseHistory, _index) << "]\",\n"
			<< "}";
		return ss;
	};
};

