#pragma once
#include "Db.h"

class Authenticator
{
public:
	static std::string* VerifyUsernameExists(const std::string& username) {
		auto db = Db::GetInstance();
		
		for (Account* obj : db->Сustomers) {
			if (obj->Name == username) {
				return &obj->password;
			}
		}
		return nullptr;
	}

	static bool VerifyLogin(const std::string& username, const std::string& password) {
		auto db = Db::GetInstance();

		for (Account* obj : db->Сustomers) {
			if (obj->Name == username && obj->password == password) {
				return true;
			}
		}
		for (Account* obj : db->Employees) {
			if (obj->Name == username && obj->password == password) {
				return true;
			}
		}
		return false;
	}
};

