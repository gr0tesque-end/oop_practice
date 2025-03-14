#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>
#include <sstream>
#include "Product.h"
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
	: public Account, public IIdentifiable
{
protected:
	float _balance;
public:
	Customer(int Id,
			 const std::string& name,
			 float balance,
			 int auth,
			 const std::string& pass)
		: Account(name, pass, auth), _balance{ balance }, IIdentifiable{ Id } {
	}

	Customer(const std::string& Id,
		const std::string& name,
		const std::string& balance,
		const std::string& auth,
		const std::string& pass) : 
		Customer{ atoi(Id.c_str()), name, static_cast<float>(atof(balance.c_str())), atoi(auth.c_str()), pass} {

	}

	// 3.1
	Customer(const Customer& c) = delete;

	// 3.2
	Customer(Customer&& c) noexcept :
		Customer{ c.id ,std::move(c.Name), c._balance, c.Authority, std::move(c.password) }
	{
		c._balance = 0;
	}

	// 5.4
	virtual ~Customer() {}

	float GetBalance() const { return this->_balance; }

	bool Buy(Product& prod, int quantity) {
		float p = prod.GetPrice() * quantity;
		if (p > _balance) { throw std::exception("Insufficient funds"); return false; }
		_balance -= p;
		prod.ChangeQuantity(-quantity);
		return true;
	}

	void ChangeBalance(float count) {
		_balance += count;
	}

	virtual const int GetId() const override { return id; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;

		ss << "\n{\n\t\"Object\": \"" << "Customer" << "\",\n"
			<< "\t\"Id\": \"" << GetId() << "\",\n"
			<< "\t\"Name\": \"" << Name << "\",\n"
			<< "\t\"Balance\": \"" << _balance << "\",\n"
			<< "\t\"Authority\": \"" << this->Authority << "\",\n"
			<< "\t\"Pass\": \"" << this->password << "\"\n" << "},";
		return ss;
	};

};

#endif // CUSTOMER_H