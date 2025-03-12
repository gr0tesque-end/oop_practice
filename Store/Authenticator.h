#pragma once
#include "Logger.h"

class Authenticator
{
public:
	static bool VerifyUsernameExists(const std::string& username) {
		auto db = Db::GetInstance();
		
		for (std::shared_ptr<Customer>& obj : db->Customers) {
			if (obj->Name == username) {
				return true;
			}
		}
		for (std::shared_ptr<Employee>& obj : db->Employees) {
			if (obj->Name == username) {
				return true;
			}
		}
		return false;
	}

	static std::shared_ptr<Account> VerifyLogin(const std::string& username, const std::string& password) {
		auto db = Db::GetInstance();

		for (std::shared_ptr<Customer>& obj : db->Customers) {
			if (obj->Name == username && obj->password == password) {
				return obj;
			}
		}

		for (std::shared_ptr<Employee>& obj : db->Employees) {
			if (obj->Name == username && obj->password == password) {
				return obj;
			}
		}

		return nullptr;
	}

	static std::shared_ptr<Employee> VerifyLogin_Employee(const std::string& username, const std::string& password) {
		auto db = Db::GetInstance();

		for (std::shared_ptr<Employee>& obj : db->Employees) {
			if (obj->Name == username && obj->password == password) {
				return obj;
			}
		}

		return nullptr;
	}
};

