#pragma once
#include <string>
#include <iostream>
class Customer
{
	const short PURCHASE_HISTORY_MAX_DEPTH = 10;

	std::string _name;
	float _balance;

	float* _PurchaseHistory = new float[PURCHASE_HISTORY_MAX_DEPTH];
	int _index = 0;

	void ReInitHistory() {
		if (_PurchaseHistory) delete[] _PurchaseHistory;
		_PurchaseHistory = new float[PURCHASE_HISTORY_MAX_DEPTH];
	}
public:
	Customer(const std::string& name = "", float balance = 0)
		: _name{ name }, _balance{ balance } {
	}

	~Customer() {
		if (_PurchaseHistory) delete[] _PurchaseHistory;
		std::cout << "~Customer\n";
	}

	bool Buy(const int& price) {
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
	const int& GetIndex() const { return _index; }
	const float* const GetHistory() const { return _PurchaseHistory; }
};

