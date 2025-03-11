#pragma once
#include "Db.h"

class Authenticator
{
public:
	static bool VerifyUsernameExists(const std::string& username) {
		auto db = Db::GetInstance();
		
		for (Account* obj : db->Customers) {
			if (obj->Name == username) {
				return true;
			}
		}
		return false;
	}

	static Account* VerifyLogin(const std::string& username, const std::string& password) {
		auto db = Db::GetInstance();

		for (Account* obj : db->Customers) {
			if (obj->Name == username && obj->password == password) {
				return obj;
			}
		}

		for (Account* obj : db->Employees) {
			if (obj->Name == username && obj->password == password) {
				return obj;
			}
		}

		return nullptr;
	}

	static Account* VerifyLogin_Employee(const std::string& username, const std::string& password) {
		auto db = Db::GetInstance();

		for (Account* obj : db->Employees) {
			if (obj->Name == username && obj->password == password) {
				return obj;
			}
		}

		return nullptr;
	}
};

