﻿#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <set>
#include <memory>

#include "Store.h"
#include <cassert>

class CLI {
public:

	Store store{};
	std::shared_ptr<Customer> acc_customer;
	std::shared_ptr<Employee> acc_employee;

	CLI() {
		Start();
	};

#pragma region Menus

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
			else if (i1 == 0)
				continue;
			else {
				return;
			}

			if (acc_customer) {
				std::cout << Message_Success << std::endl;
				MenuCustomer();
				system("cls");
			}
			else if (acc_employee) {
				std::cout << Message_Success << std::endl;
				MenuEmployee();
				system("cls");
			}
		}
	}
	
	// imp
	void MenuCustomer() {
		while (true) {

			std::cout << "[Menu]" << std::endl
				<< "Msg > Welcome, " << acc_customer->GetName() << std::endl
				<< "Balance > " << acc_customer->GetBalance() << std::endl;
			int i1 = optionInput({
				"Catalog of products",  // 1 # Implemented
				"Buy a product",        // 2 # Implemented + logging
				"Deposit",              // 3 # Implemented + logging
				"Purchase History",     // 4 # Implemented
				"Refunding"				// 5 # Implemented + logging
				});

			switch (i1) {
			case 0:
				continue;
				break;
			case 1:
				std::cout << store.Catalog().str();
				break;
			case 2:
			{
				store.BindCustomer(acc_customer);
				manualInput<Store>({ "Product Id", "Quantity" }, std::mem_fn(&Store::Purchase));
				store.UnbindCustomer();
				break;
			}
			case 3:
				store.BindCustomer(acc_customer);
				manualInput<Store>({ "Amount" }, std::mem_fn(&Store::Deposit));
				store.UnbindCustomer();

				break;
			case 4:
				GetPurchaseHistory();
				break;
			case 5:
				MenuCustomer_Refunding();
				break;
			default:
				return;
			}
		}
	}

	void MenuCustomer_Refunding() {
		system("cls");
		while (true)
		{
			std::cout << "[Refunding]" << std::endl;
			int i1 = optionInput({
				"Request refund", // imp
				"See processed refunds", // imp
				"See pending refunds", // imp
				"Purchase History" // imp
				});

			switch (i1)
			{
			case 0:
				continue;
				break;
			case 1:
				store.BindCustomer(acc_customer);
				manualInput<Store>({ "Purchase Id" }, std::mem_fn(&Store::RefundReq));
				store.UnbindCustomer();
				break;
			case 2:
				for (auto l : store.db->SearchAll<Log>([](std::shared_ptr<Log> l) {
					auto desc = l->GetDescription();
					return desc.ends_with("deny") || desc.ends_with("accept");
					})) std::cout << store.Log_to_string(l);
					break;
			case 3:
				for (auto l : store.db->SearchAll<Log>([](std::shared_ptr<Log> l) {
					auto desc = l->GetDescription();
					return desc.ends_with("req");
					})) std::cout << store.Log_to_string(l);
				break;
			case 4:
				GetPurchaseHistory();
				break;
			default:
				return;
				break;
			}
		}
	}

	// imp
	void MenuEmployee() {
		while (true)
		{
			std::cout << "[Start]" << std::endl
				<< "Msg > Welcome, " << acc_employee->GetName()
				<< " [Employee]" << std::endl;
			int i1 = optionInput({
				"Products menu",	// imp
				"Search Object",    // imp
				"Refunds menu"	    // imp
				});

			switch (i1)
			{
			case 0:
				continue;
				break;
			case 1:
				MenuEmployee_Products();
				break;
			case 2: {
				Search(nullptr);
				break;
			}
			case 3:
				MenuEmployee_Refunds();
				break;
			default:
				return;
			}
		}
	}

	// imp
	void MenuEmployee_Products() {
		system("cls");
		while (true)
		{
			std::cout << "[Products]" << std::endl;
			int i1 = optionInput({
				"Modify Product(s)", // imp
				"Create Product",    // # Implemented /w logging 
				"See Products",		 // # imp
				"Search"
				});

			switch (i1)
			{
			case 0:
				continue;
				break;
			case 1:
				{
				auto f = [&](const std::vector<std::string>& data) {
					store.BindEmployee(acc_employee);
					int i2 = optionInput({
						"rename",
						"restock",
						"reprice"
						});
					if (i2 > 3 || i2 < 1) return;
					manualInput<Store>({
						data[0],
						std::to_string(i2),
						(i2 == 1) ? "New Name" :
						(i2 == 2) ? "New Quantity" :
						"New Price"
						}, std::mem_fn(&Store::ModProduct), 2);
					store.UnbindEmployee();
					};
				manualInput({ "Id" }, f);
				break;
			}
			case 2:
				store.BindEmployee(acc_employee);
				manualInput<Store>({
					"Name", "Price", "Stock"
					}, std::mem_fn(&Store::CreateProduct));
				store.UnbindEmployee();
				break;
			case 3:
				std::cout << store.Catalog().str() << std::endl;
				break;
			case 4:
				Search("Product");
				break;
			default:
				return;
			}
		}
	}
	// imp
	void MenuEmployee_Refunds() {
		system("cls");
		while (true)
		{
			auto refunds = store.SearchLogs([](std::shared_ptr<Log> l)-> bool {
				return *l == "Refund_req";
				});

			std::cout << "[Refunds]" << std::endl;
			int i1 = optionInput({
				"Process Refund",		// imp
				"See pending Refunds",	// imp
				"See processed Refunds" // imp
				});

			switch (i1)
			{
			case 1:
				store.BindEmployee(acc_employee);
				manualInput<Store>({
					"Refund Id", "Answer (y/n)"
					}, std::mem_fn(&Store::ProcessRefund));
				store.UnbindEmployee();
				break;
			case 2:
				for (auto r : refunds) std::cout << store.Log_to_string(r) << std::endl;
				break;
			case 3:
				for (auto r : store.SearchLogs([](auto l) {
					return *l == "Refund_accept" || *l == "Refund_deny";
					})) std::cout << store.Log_to_string(r) << std::endl;
				break;
			default:
				return;
			}
		}
	}
#pragma endregion

#pragma region Input Processing
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

			if (choice.starts_with("clear")) {
				system("cls");
				return 0;
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
		ProcessingMethod<T> nextCall,
		int pos = 0) {

		bool i = pos > 0;

		for (; pos < inputParams.size();) {
			std::string loc;
			std::cout << inputParams[pos] << " (.q to cancel) > ";
			std::getline(std::cin, loc);

			if (loc.starts_with(".q")) {
				return;
			}

			inputParams[pos] = loc;
			++pos;
		}

		/*if (!additionalData.empty()) {
			inputParams.insert(inputParams.end(), additionalData.begin(), additionalData.end());
		}*/
		if constexpr (std::is_same_v<T, CLI>) {
			nextCall(*this, inputParams);
		}
		else if constexpr (std::is_same_v<T, Store>) {
			nextCall(store, inputParams);
		}
	}

	void manualInput(std::vector<std::string> inputParams,
		ProcessingAction nextCall) {

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

		nextCall(inputParams);
	}
#pragma endregion

#pragma region Helper Functions
private:
	std::shared_ptr<Account> _LogIn(const std::string& name, const std::string& pass) {
		return Authenticator::VerifyLogin(name, pass);
	}

	void Search(const char* what) {
		std::vector<std::string> options;
		options.push_back((what) ? what : "Object Name (e.g. Log, Customer or Product)");

		options.push_back(R"(Possible options: 
Any:
	Id
Customer: 
	Name,
	Balance
Employee:
	Name,
	Authority
Product:
	Name,
	Price,
	Quantity
Logs:	
	Description,
	Executionerid,
	CreatedBy (object type)
Parameter name (can enter first letter))");

		manualInput<Store>(options, std::mem_fn(&Store::BuildSearchFunc), (what) ? 1 : 0);

		manualInput(
			{ "Parameter value (can use comparason with the integral types;\nWhen comparing integral types, type '(' before your expression\ndouble operators e.g. >= not allowed\ne.g \"(>4.43\", \"(<33\", \"(!3\" '!' for the 'not equal')" },
			store.SearchFunc);


		std::cout << "Result: " << std::endl;
		for (const std::string& res : store.search_result)
		{
			std::cout << res;
		}

		std::cout << "\nFound: " << store.search_result.size() << " occurences" << std::endl;
		store.search_result.clear();
	}

	void LogIn(const std::vector<std::string>& data) {
		acc_customer = nullptr;
		acc_employee = nullptr;
		auto acc = _LogIn(data[0], data[1]);
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
		if (!(bool(acc_employee) ^ bool(acc_customer))) {
			throw std::exception("!(acc_employee ^ acc_customer)");
		}
	}

	void SignUp(const std::vector<std::string>& data) {
		acc_customer = nullptr;
		acc_employee = nullptr;

		if (data[1] != data[2]) {
			std::cout << Message_Fail << " > Passwords must match" << std::endl;
			return;
		}

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
			std::cout << Message_Fail << " > Username already exists!" << std::endl;
			return;
		}

		if (match.size() > 2 && match.size() <= 8) {

			if (match[3] == "reqby") {
				auto temp = _LogIn(match[4].str(), match[5].str());
				if (temp->GetAuthority() >= AuthorityEmployeeThreshhold) {
					int newAuth = atoi(match[7].str().c_str());
					if (newAuth >= AuthorityEmployeeThreshhold) {
						acc_employee = std::make_shared<Employee>(-1, match[1].str(), 0.f, newAuth, data[1]);
						store.db->push_back(acc_employee);
						store.db->Flush<Employee>();
					}
					else {
						acc_customer = std::make_shared<Customer>(-1, match[1].str(), 0.f, newAuth, data[1]);
						store.db->push_back(acc_customer);
						store.db->Flush<Customer>();
					}
				}
			}
			else {
				SignUp_BaseCase({ match[1].str(), data[1] });
			}
			// [Expand here]
		}
		else SignUp_BaseCase(data);

		if (!(bool(acc_employee) ^ bool(acc_customer))) {
			throw std::exception("!(acc_employee ^ acc_customer)");
		}
	}
	void SignUp_BaseCase(const std::vector<std::string>& data) {
		if (!acc_customer) {
			acc_customer = std::make_shared<Customer>(-1, data[0], 0.f, 0, data[1]);
			store.db->push_back(acc_customer);
			store.db->Flush<Customer>();
		}
	}

	void GetPurchaseHistory() {
		for (auto ptr : store.db->SearchAll<Log>([&](std::shared_ptr<Log> l) {
			return l->GetDescription() == "Purchase" && *l < "Customer" && *l < acc_customer->GetId();
			})) std::cout << store.Log_to_string(ptr);
	}
#pragma endregion

};