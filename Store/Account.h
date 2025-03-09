#pragma once
#include "INameable.h"

class Account
	: public INameable
{	
protected:
	int Authority{ 0 };
	std::string password{};
public:
	Account(const std::string& name, const std::string& pass, int Auth) 
		: INameable{ name }, password{ pass }, Authority{ Auth } {
	}
	
	void SetPassword(std::string newPass, const Account& changer) {
		if (password.empty() || changer.Authority >= 100) {
			password = newPass;
			std::cout << "Success. [passchange]\n";
		}
		std::cout << "Enter Current Password > "; 
		std::string cp;
		std::cin >> cp;
		if (cp == password) {
			password = newPass;
			std::cout << "Success. [passchange]\n";
		}
		else {
			std::cout << "Failed. [passchange]\n";
		}
	}

	void SetAuthority(int newAuthority, const Account& changer) {
		if (changer.Authority >= 100) {
			Authority = newAuthority;
			std::cout << "Success. [authoritychange]\n";
		}
		else {
			std::cout << "Failed. [authoritychange]\n";
		}
	}

	int GetAuthority() const { return Authority; }

	friend class Authenticator;
};

