#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>
#include <sstream>
#include "INameable.h"
#include "IIdentifiable.h"
#include "Misc.h"
#include <map>

enum SubPlans;

enum SubPlans : short
{
	Basic = 4,
	Advanced = 6,
	Premium = 10
};

class Customer
	: public INameable, public IIdentifiable
{
protected:
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
		: INameable( name ), _balance{ balance } {
	}

	// 3.1
	Customer(const Customer& c) :
		INameable( c.Name ),
		_balance{ c._balance }, _index{ c._index }
	{
		_PurchaseHistory = new float[PURCHASE_HISTORY_MAX_DEPTH];
		for (int i = 0; i < _index; i++)
		{
			_PurchaseHistory[i] = c._PurchaseHistory[i];
		}
	}
	// 3.2
	Customer(Customer&& c) noexcept :
		INameable{ std::move(c.Name) },
		_balance{ c._balance }, _index{ c._index },
		_PurchaseHistory{ c._PurchaseHistory }
	{
		c._PurchaseHistory = nullptr;
		c._index = 0;
		c._balance = 0;
	}
	// 5.4
	virtual ~Customer() {
		if (_PurchaseHistory) delete[] _PurchaseHistory;
		std::cout << "~Customer\n";
	}

	virtual bool Buy(const float& price) {
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

		ss << "\n{\n\tObject: \"" << "Customer" << "\",\n"
			<< "\tId: \"" << GetId() << "\",\n"
			<< "\tName: \"" << Name << "\",\n"
			<< "\tBalance: \"" << _balance << "\",\n"
			<< "\tPurchase(s): \"[ " << Misc::ArrToStr<float*>(_PurchaseHistory, _index) << " ]\"\n"
			<< "}";
		return ss;
	};

};

#endif // CUSTOMER_H