#pragma once
#include "Authenticator.h"

const char* Message_Success = "Msg > Success";
const char* Message_Fail = "Msg > Fail";

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

	void CreateProduct(const std::vector<std::string>& data /* Name, Price, Stock*/) {
		if (!acc_employee) {
			LatestReq = false;
			return;
		}

		std::shared_ptr<Log> log = std::make_shared<Log>(
			-1,
			2,
			std::vector<std::string>{},
			"Create new Product",
			acc_employee->GetId());

		try
		{
			db->push_back(std::make_shared<Product>("-1", data[0], data[1], data[2]))
				->Flush<Product>();
			log->AddArg(Message_Success);
			std::cout << Message_Success << std::endl;
			
		}
		catch (const std::exception&)
		{
			LatestReq = false;
			log->AddArg(Message_Fail);
			std::cout << Message_Fail << std::endl;
			return;
		}
		db->push_back(log)
			->Flush<Log>();
		LatestReq = true;
	}

	Store& BindCustomer(std::shared_ptr<Customer>& customer) { acc_customer = customer; return *this;  }
	void UnbindCustomer() { acc_customer = nullptr; }

	Store& BindEmployee(std::shared_ptr<Employee>& employee) { acc_employee = employee; return *this; }
	void UnbindEmployee() { acc_employee = nullptr; }

	std::stringstream Catalog() const {
		std::stringstream res;
		for (auto p: db->Products)
		{
			res << p->ToString().str();
		}
		return res;
	}

	friend class CLI;
};

