#pragma once
#include "Authenticator.h"

class Store
{
	Db* db = Db::GetInstance();
	
	std::shared_ptr<Customer> acc_customer;
	std::shared_ptr<Employee> acc_employee;
public:
	bool LatestReq;

	void Purchase(const std::vector<std::string>& data /* Id & Quantity + customer_id */) {
		if (!acc_customer) {
			LatestReq = false;
			return;
		}

		int id = atoi(data[0].c_str());

		LatestReq = acc_customer->Buy(*db->SearchProduct(id), atoi(data[1].c_str()));
	}

	void Deposit(const std::vector<std::string>& data /* float */) {
		if (!acc_customer) {
			return;
		}

		acc_customer->ChangeBalance(atof(data[0].c_str()));
	}

	void BindCustomer(std::shared_ptr<Customer> customer) { acc_customer = customer; }
	void UnbindCustomer() { acc_customer = nullptr; }

	void BindEmployee(std::shared_ptr<Employee> employee) { acc_employee = employee; }
	void UnbindEmployee() { acc_employee = nullptr; }

	std::stringstream Catalog() const {
		std::stringstream res;
		for (auto p: db->Products)
		{
			res << p->ToString().str() << ',';
		}
		return res;
	}

	friend class CLI;
};

