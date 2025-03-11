#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <set>
#include <memory>

#include "Authenticator.h"

class CLI {
private:
	std::shared_ptr<Account> _LogIn(const std::string& name, const std::string& pass) {
		return std::shared_ptr<Account>(Authenticator::VerifyLogin(name, pass));
	}
public:
	using Action = std::function<void(CLI&)>;
	using ProcessingAction = std::function<void(CLI&, const std::vector<std::string>&)>;

	Action dc;

	Db* db = Db::GetInstance();
	std::shared_ptr<Account> acc;

	bool isEmployee{ false };

	CLI() {
		Start();
	};

	void Start() {
		std::mem_fn(&CLI::Start);
		std::cout << "[Start]" << std::endl
			<< "Msg > Welcome to <Store>" << std::endl;

		int i1 = optionInput({ "LogIn", "SignUp" });

		// Auth
		if (i1 == 1) manualInput({ "User", "Password" }, std::mem_fn(&CLI::LogIn));
		else manualInput({ "User", "Password", "Repeat Password" }, std::mem_fn(&CLI::SignUp));

		if (acc) std::cout << "Msg > Success" << std::endl;

		if (isEmployee) MenuEmployee();
		else MenuCustomer();
	}

	void MenuCustomer() {
		system("cls");
		std::mem_fn(&CLI::MenuCustomer);
		std::cout << "[Menu]" << std::endl
			<< "Msg > Welcome, " << acc->GetName() << std::endl;
		int i1 = optionInput({
			"Catalog of products",
			"Buy a product",
			"Deposit",
			"Balance",
			"History",
			"Log out"
			});
	}

	void MenuEmployee() {
		system("cls");
		dc = std::mem_fn(&CLI::MenuEmployee);
		std::cout << "[Start]" << std::endl
			<< "Msg > Welcome, " << acc->GetName() << " [Employee]" << std::endl;
		int i1 = optionInput({
			"Modify Account(s)",
			"Modify Product(s)"
			"See Logs",
			"Log out"
			});
	}

	void LogIn(const std::vector<std::string>& data) {
		acc = _LogIn(data[0], data[1]);
		if (!acc) {
			std::cout << "Msg > Login failed. Try again.\n";
			manualInput({ "User", "Password" }, std::mem_fn(&CLI::LogIn));
		}
		else {
			if (acc->GetAuthority() >= 100) isEmployee = true;
		}
	}

	void SignUp(const std::vector<std::string>& data) {
		// ^(\w+)(\<(\w+)\>\["(\w+)", ?"(\w+)"\])?$
		std::regex pattern{
			"^(\\w+)(\\<(\\w+)\\>\\[\"(\\w+)\", ?\"(\\w+)\"(, ?\"(\\w+)\")?\\])?$",
			std::regex_constants::ECMAScript
		};
		std::smatch match;

		std::list<std::string> result{};

		auto begin = data[0].cbegin();
		auto end = data[0].cend();

		std::regex_search(begin, end, match, pattern);

		// Username<reqby>["Admin", "admin", "100"] 
		// "Username" at 1
		// "CreatedBy" at 3
		// "User" and "pass" at 4 and 5
		// "100" at 7

		if (Authenticator::VerifyUsernameExists(match[1])) {
			std::cout << "Username already exists!" << std::endl;
			manualInput({ "User", "Password", "Repeat Password" }, std::mem_fn(&CLI::SignUp));
		}

		if (match.size() > 2 && match.size() <= 8) {

			if (match[3] == "reqby") {
				auto temp = _LogIn(match[4].str(), match[5].str());
				// Change here to set the threshhold for authority assignment by user
				if (temp->GetAuthority() >= 100) {
					int newAuth = atoi(match[7].str().c_str());
					if (newAuth >= 100) {
						acc = std::make_shared<Employee>(-1, match[1].str(), 0.f, newAuth, data[1]);
						db->Employees.push_back(dynamic_cast<Employee*>(acc.get()));
						isEmployee = true;
					}
					else {
						acc = std::make_shared<Customer>(-1, match[1].str(), 0.f, nullptr, 0, newAuth, data[1]);
						db->Customers.push_back(dynamic_cast<Customer*>(acc.get()));
					}
				}
			}
			// [Expand here]
		}
		if (match.size() >= 1) {
			if (!acc) {
				acc = std::make_shared<Customer>(-1, match[1].str(), 0.f, nullptr, 0, 0, data[1]);
				db->Customers.push_back(dynamic_cast<Customer*>(acc.get()));
			}
		}
	}

	int optionInput(std::vector<std::string> options) {
		for (int i{ 0 }; i < options.size(); ++i) {
			std::cout << i + 1 << ". " << options[i] << std::endl;
		}
		std::string choice;
		std::cout << "> ";
		std::getline(std::cin, choice);

		int c = std::stoi(choice);

		if (c > 0 && c <= options.size()) {
			return c;
		}
		else {
			std::cout << "Msg > Invalid choice. Try again.\n";
			return optionInput(options);
		}
	}

	void manualInput(std::vector<std::string> inputParams, ProcessingAction nextCall) {
		int i{};
		for (const auto& o : inputParams) {
			std::string loc;
			std::cout << o << " (.q to cancel) > ";
			std::getline(std::cin, loc);

			if (loc.starts_with(".q")) {
				return;
			}

			inputParams[i] = loc;
			++i;
		}
		nextCall(*this, inputParams);
	}
};