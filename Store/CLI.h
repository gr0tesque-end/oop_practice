#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <set>
#include <memory>

#include "Store.h"

class CLI;

const char* Message_Success = "Msg > Success";

template <typename T>
concept IsAuthorisedClass =
std::is_same_v<T, CLI> ||
std::is_same_v<T, Store>;

template<IsAuthorisedClass AC>
using ProcessingAction = std::function<void(AC&, const std::vector<std::string>&)>;

class CLI {
private:
	std::shared_ptr<Account> _LogIn(const std::string& name, const std::string& pass) {
		return std::shared_ptr<Account>(Authenticator::VerifyLogin(name, pass));
	}

public:

	Store store{};
	std::shared_ptr<Account> acc;
	std::shared_ptr<Customer> acc_customer;
	std::shared_ptr<Employee> acc_employee;

	CLI() {
		Start();
	};

	void Start() {
		while (true) {

			acc_customer = nullptr;
			acc_employee = nullptr;

			std::cout << "[Start]" << std::endl
				<< "Msg > Welcome to <Store>" << std::endl;

			int i1 = optionInput({ "LogIn", "SignUp", "Exit" });

			if (i1 == 1) {
				manualInput<CLI>({ "User", "Password" }, std::mem_fn(&CLI::LogIn));
			}
			else if (i1 == 2) {
				manualInput<CLI>({ "User", "Password", "Confirm Password" }, std::mem_fn(&CLI::SignUp));
			}
			else {
				return;
			}

			if (acc) {
				std::cout << Message_Success << std::endl;
				system("cls");
				if (acc_employee) {
					//store.employee = acc_employee;
					MenuEmployee();
				}
				else if (acc_customer) {
					//store.customer = acc_customer;
					MenuCustomer();
				}
			}
		}
	}

	void MenuCustomer() {
		while (true) {

			std::cout << "[Menu]" << std::endl
				<< "Msg > Welcome, " << acc->GetName() << std::endl
				<< "Balance > " << acc_customer->GetBalance() << std::endl;
			int i1 = optionInput({
				"Catalog of products",  // 1 # Implemented
				"Buy a product",        // 2 # Implemented
				"Deposit",              // 3 # Implemented
				"History",              // 4 # Need Logging
				"Refund Req"			// 5 # Need Logging
				});

			switch (i1) {
			case 1:
				std::cout << store.Catalog().str();
				break;
			case 2:
			{
				store.BindCustomer(acc_customer);
				manualInput<Store>({ "Product_Id", "Quantity" }, std::mem_fn(&Store::Purchase));
				store.UnbindCustomer();

				if (store.LatestReq) {
					std::cout << Message_Success << std::endl;
				}
				break;
			}
			case 3:
				store.BindCustomer(acc_customer);
				manualInput<Store>({ "Amount" }, std::mem_fn(&Store::Deposit));
				store.UnbindCustomer();

				// Log here

				break;
			default:
				return;
			}
		}
	}

	void MenuEmployee() {
		while (true)
		{
			std::cout << "[Start]" << std::endl
				<< "Msg > Welcome, " << acc->GetName()
				<< " [Employee]" << std::endl;
			int i1 = optionInput({
				"Modify Account(s)",
				"Modify Product(s)"
				"See Logs",
				"See Products",
				"See Accounts"
				});

			switch (i1)
			{
			case 1:

			default:
				return;
			}
		}
	}

	int optionInput(std::vector<std::string> options) {
		while (true) {
			for (int i{ 0 }; i < options.size(); ++i) {
				std::cout << i + 1 << ". " << options[i] << std::endl;
			}
			std::string choice;
			std::cout << "(.q to cancel) > ";
			std::getline(std::cin, choice);

			if (choice.starts_with(".q")) {
				return -1;
			}

			try {
				int c = std::stoi(choice);
				if (c > 0 && c <= options.size()) {
					return c;
				}
				else {
					std::cout << "Msg > Invalid choice. Try again.\n";
				}
			}
			catch (std::invalid_argument&) {
				std::cout << "Msg > Invalid input. Please enter a number.\n";
			}
		}
	}

	template<IsAuthorisedClass T>
	void manualInput(std::vector<std::string> inputParams,
		ProcessingAction<T> nextCall) {

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

		/*if (!additionalData.empty()) {
			inputParams.insert(inputParams.end(), additionalData.begin(), additionalData.end());
		}*/
		if constexpr (std::is_same_v<T, CLI>) {
			nextCall(*this, inputParams);
		}
		else {
			nextCall(store, inputParams);
		}
	}

	void LogIn(const std::vector<std::string>& data) {
		acc = _LogIn(data[0], data[1]);
		if (!acc) {
			std::cout << "Msg > Login failed. Try again.\n";
		}
		else {
			if (acc->GetAuthority() >= AuthorityEmployeeThreshhold) {
				acc_employee = dynamic_pointer_cast<Employee, Account>(acc);
			}
			else {
				acc_customer = dynamic_pointer_cast<Customer, Account>(acc);
			}
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
			manualInput<CLI>({ "User", "Password", "Repeat Password" }, std::mem_fn(&CLI::SignUp));
		}

		if (match.size() > 2 && match.size() <= 8) {

			if (match[3] == "reqby") {
				auto temp = _LogIn(match[4].str(), match[5].str());
				if (temp->GetAuthority() >= AuthorityEmployeeThreshhold) {
					int newAuth = atoi(match[7].str().c_str());
					if (newAuth >= AuthorityEmployeeThreshhold) {
						acc = std::make_shared<Employee>(-1, match[1].str(), 0.f, newAuth, data[1]);
						acc_employee = dynamic_pointer_cast<Employee, Account>(acc);
						store.db->Employees.push_back(acc_employee.get());
					}
					else {
						acc = std::make_shared<Customer>(-1, match[1].str(), 0.f, nullptr, 0, newAuth, data[1]);
						acc_customer = dynamic_pointer_cast<Customer, Account>(acc);
						store.db->Customers.push_back(acc_customer.get());
					}
				}
			}
			// [Expand here]
		}
		if (match.size() >= 1) {
			if (!acc) {
				acc = std::make_shared<Customer>(-1, match[1].str(), 0.f, nullptr, 0, 0, data[1]);
				store.db->Customers.push_back(dynamic_cast<Customer*>(acc.get()));
			}
		}
	}
};