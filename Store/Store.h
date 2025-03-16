#pragma once
#include "Authenticator.h"

const char* Message_Success = "Msg > Success";
const char* Message_Fail = "Msg > Fail";

const char* Customer_NotBound = " > Customer not bound";
const char* Employee_NotBound = " > Employee not bound";
const char* Account_NotBound = " > Account not bound";

const char* Log_NotFound = " > Log not found";
const char* Product_NotFound = Log_NotFound;
const char* Purchase_NotFound = Log_NotFound;
const char* Type_NotFound = " > Unrecognised type > ";

class CLI;
class Store;

template <typename T>
concept IsAuthorisedClass =
std::is_same_v<T, CLI> ||
std::is_same_v<T, Store>;

template<IsAuthorisedClass AC>
using ProcessingMethod = std::function<void(AC&, const std::vector<std::string>&)>;

using ProcessingAction = std::function<void(const std::vector<std::string>&)>;

class Store
{
	Db* db = Db::GetInstance();

	std::shared_ptr<Customer> acc_customer;
	std::shared_ptr<Employee> acc_employee;

public:
	std::list<std::string> search_result;

	ProcessingAction SearchFunc;

#pragma region C-Bound methods
	// c-bound
	void RefundReq(const std::vector<std::string>& data) {
		if (!acc_customer) {
			std::cout << Message_Fail << " > " << Customer_NotBound << std::endl;
			return;
		}

		int id = atoi(data[0].c_str());
		int cusId = acc_customer->GetId();

		// Check if Refund exists
		if (auto r = db->Search<Log>([&](std::shared_ptr<Log> l) {
			auto desc = l->GetDescription();
			bool t = desc.starts_with("Refund");
			return t &&
				l->HasArg(data[0]);
			})) {
			std::cout << Message_Fail << " > " << "Refund has already been requested" << std::endl;
			return;
		}

		auto l = db->Search<Log>([cusId, id](auto l) {
			return l->GetDescription() == "Purchase" &&
				(l->ExecutionerId == cusId && l->id == id);
			});

		if (!l) { std::cout << Message_Fail << Purchase_NotFound << std::endl; return; }

		std::list<char> prodId{};
		for (auto rbegiter = (l->args[2]).rbegin(); *rbegiter != ' '; ++rbegiter) {
			prodId.push_front(*rbegiter);
		}
		float price{};
		if (auto p = db->Search<Product>([prodId](auto prod) {
			return prod->GetId() == atoi(std::string{
				prodId.begin(),
				prodId.end() }
			.c_str());
			})
			) {
		}
		else {
			std::cout << Message_Fail << " > " << Product_NotFound << std::endl;
			return;
		}

		std::shared_ptr<Log> log = std::make_shared<Log>(
			-1,
			4,
			std::vector<std::string>{ data[0] },
			"Refund_req",
			acc_customer->GetId(),
			"Customer");
		log->AddArg(l->args[1].c_str());

		std::cout << Message_Success << std::endl;

		db->push_back(log)
			->Flush<Log>(true);
	}
	// c-bound
	void Purchase(const std::vector<std::string>& data /* Id & Quantity + customer_id */) {
		if (!acc_customer) {
			std::cout << Message_Fail << " > " << Customer_NotBound << std::endl;
			return;
		}

		int id = atoi(data[0].c_str());

		auto p = db->Search<Product>([id](auto prod) {
			return prod->GetId() == id;
			});

		if (!p) {
			std::cout << Message_Fail << " > " << Product_NotFound << std::endl;
			return;
		}
		try
		{
			int count = atoi(data[1].c_str());
			acc_customer->Buy(*p, count);


			std::shared_ptr<Log> log = std::make_shared<Log>(
				-1,
				0,
				std::vector<std::string>{},
				"Purchase",
				acc_customer->GetId(),
				"Customer");
			log->AddArg(Message_Success)
				->AddArg(std::to_string(count * p->GetPrice()))
				->AddArg(std::string{ "ProductId: " + std::to_string(p->GetId()) });
			std::cout << Message_Success << std::endl;
			db->push_back(log)
				->Flush<Log>(true)
				->Flush<Customer>()
				->Flush<Product>();
		}
		catch (const std::exception& ex)
		{
			std::cout << Message_Fail << " > " << ex.what() << std::endl;
			return;
		}

	}

	// c-bound
	void Deposit(const std::vector<std::string>& data /* float */) {
		if (!acc_customer) {
			std::cout << Message_Fail << " > " << Customer_NotBound << std::endl;
			return;
		}

		float how_much = atof(data[0].c_str());

		std::shared_ptr<Log> log = std::make_shared<Log>(
			-1,
			3,
			std::vector<std::string>{ data[0] },
			"Deposit",
			acc_customer->GetId(),
			"Customer");

		acc_customer->ChangeBalance(how_much);

		db->push_back(log)
			->Flush<Log>(true)
			->Flush<Customer>();

		std::cout << Message_Success << std::endl;
	}
#pragma endregion

#pragma region E-Bound methods
	// e-bound
	void CreateProduct(const std::vector<std::string>& data /* Name, Price, Stock*/) {
		if (!acc_employee) {
			std::cout << Message_Fail << " > " << Employee_NotBound << std::endl;
			return;
		}

		std::shared_ptr<Log> log = std::make_shared<Log>(
			-1,
			2,
			std::vector<std::string>{ },
			"Create new Product",
			acc_employee->GetId(),
			"Employee");

		try
		{
			auto p = std::make_shared<Product>("-1", data[0], data[1], data[2]);
			db->push_back(p)
				->Flush<Product>(true);
			log->AddArg(Message_Success)
				->AddArg(std::string{ "ProductId: " + std::to_string(p->GetId()) });
			std::cout << Message_Success << std::endl;

		}
		catch (const std::exception&)
		{
			log->AddArg(Message_Fail);
			std::cout << Message_Fail << std::endl;
			return;
		}
		db->push_back(log)
			->Flush<Log>(true);
	}

	// e-bound
	void ProcessRefund(const std::vector<std::string>& data) {
		if (!acc_employee) {
			std::cout << Message_Fail << " > " << Employee_NotBound << std::endl;
			return;
		}
		int logId = atoi(data[0].c_str());

		auto log = db->Search<Log>([logId](std::shared_ptr<Log> log)-> bool {
			return log->GetId() == logId && *log == "Refund_req";
			});

		if (!log) {
			std::cout << Message_Fail << " > " << Log_NotFound << std::endl;
			return;
		}

		std::string is = (std::tolower(data[1][0]) == 'y' ? "Refund_accept" : "Refund_deny");

		log->Description.clear();
		log->Description << is;

		log->AddArg(std::string{ "ProcessedBy: " } + std::to_string(acc_employee->GetId()));


		if (is.back() == 'y') {

		}
		else {
			auto c = db->Search<Customer>([log](std::shared_ptr<Customer> customer) {
				return customer->GetId() == log->ExecutionerId;
				});
			float price = atof(log->args[1].c_str());
			if (c)
			{
				c->ChangeBalance(price);
			}
			int id = atoi(data[0].c_str());
			auto p = db->Search<Product>([id](auto prod) {
				return prod->GetId() == id;
				});
			if (p) {
				int quantity = p->GetPrice() / price;
				p->ChangeQuantity(quantity);
			}
			db->Flush<Customer>()
				->Flush<Product>();
		}

		std::cout << Message_Success << std::endl;

		db->Flush<Log>();
	}

	// e-bound
	void ModProduct(const std::vector<std::string>& data /* id, Action{ Rename, Restock, Reprice }, New Value */) {
		if (!acc_employee) {
			std::cout << Message_Fail << " > " << Employee_NotBound << std::endl;
			return;
		}
		int targetId = atoi(data[0].c_str());
		auto p = db->Search<Product>([targetId](auto p) {
			return p->GetId() == targetId;
			});

		if (!p) {
			std::cout << Message_Fail << " > " << Product_NotFound << std::endl;
			return;
		}

		std::function<const char* (char n, const std::string& newV)> f = [p](char n, const std::string& newV) -> const char* {
			if (n == '1') {
				p->Rename(newV);
				return "Rename";
			}
			else if (n == '2') {
				p->SetQuantity(atoi(newV.c_str()));
				return "Restock";
			}
			else if (n == '3') {
				p->SetPrice((float)atof(newV.c_str()));
				return "Reprice";
			}
			else {
				return nullptr;
			}
			};
		const char* ac = f(data[1][0], data[2]);

		if (!ac) {
			std::cout << Message_Fail << " > " << "Invalid action" << std::endl;
			return;
		}

		std::shared_ptr<Log> l = std::make_shared<Log>(-1, 2, std::vector <std::string> { Message_Success, (std::string{"ProductId: "} + std::string{data[0]}) },
			std::string{ ac }, acc_employee->GetId(), std::string{ "Employee" });

		std::cout << Message_Success << std::endl;
		db->push_back(l)
			->Flush<Log>(true)
			->Flush<Product>();
	}
#pragma endregion

#pragma region Binding Methods
	Store& BindCustomer(std::shared_ptr<Customer>& customer) { acc_customer = customer; return *this; }
	void UnbindCustomer() { acc_customer = nullptr; }

	Store& BindEmployee(std::shared_ptr<Employee>& employee) { acc_employee = employee; return *this; }
	void UnbindEmployee() { acc_employee = nullptr; }
#pragma endregion

	std::stringstream Catalog() const {
		std::stringstream res;
		for (auto p : db->Products)
		{
			res << " ---------" << std::endl
				<< "| ProductId: " << p->GetId() << std::endl
				<< "| Name: " << p->GetName() << std::endl
				<< "| Price: " << p->GetPrice() << std::endl
				<< "| Stock: " << p->GetQuantity() << std::endl;
		}
		return res;
	}

	void BuildSearchFunc(const std::vector<std::string>& data) {
		const auto& type = data[0], & param = data[1];
		bool isIntegral = false;
		char op{ '=' };

		float searchVal{};
		// I'm definetely going to prison for this one
		SearchFunc = [&, type, param](const std::vector<std::string>& dataInner) {
			if (isIntegral = dataInner[0][0] == '(') {
				char first = dataInner[0][1];

				searchVal = static_cast<float>(atof(dataInner[0].c_str() + 2));
				switch (first)
				{
				case '>':
					op = '>';
					break;
				case '<':
					op = '<';
					break;
				case '!':
					op = '!';
					break;
				default:
					break;
				};
			}
			for (auto& item : db->SearchAll(type.c_str(), [param, isIntegral, op, searchVal, dataInner](auto obj) -> bool {
				auto objData = split(obj->ToString().str(), '"');

				bool foundParam{ 0 };

				for (int i{ -1 }; const auto & p : objData) {
					++i;
					if (foundParam) {
						// Value
						if (i % 4 == 3) {
							if (!isIntegral) return dataInner[0] == p;

							float val{ static_cast<float>(atof(p.c_str())) };

							switch (op)
							{
							case '>':
								return isLess(val, searchVal);
								break;
							case '<':
								return isBigger(val, searchVal);
								break;
							case '!':
								return !areEqual(val, searchVal);
								break;
							default:
								return areEqual(val, searchVal);
								break;
							}
						}
						continue;
					}
					// Key
					if (i % 4 == 1) {
						foundParam = toupper(param[0]) == p[0];
					}
				}
				return false;
				}))
			{
				search_result.push_back(item->ToString().str());
			}
		};
	}

	// (c|e)-bound
	std::stringstream Get_AcountRelated_Logs_str() const {
		std::stringstream res{};

		if (!acc_customer && !acc_employee) {
			res << Message_Fail << " > " << Account_NotBound << std::endl;
			return res;
		}

		int id = (acc_customer ? acc_customer->GetId() : acc_employee ? acc_customer->GetId() : -1);
		if (id == -1) return res;
		for (std::shared_ptr<Log> l : db->Logs) {
			if (l->ExecutionerId != id) continue;
			res << " ---------" << std::endl
				<< "| LogId: " << l->GetId() << std::endl
				<< "| Action: " << l->action << std::endl
				<< "| Description: " << l->Description.str() << std::endl
				<< "| Args: " << Misc::ArrToStr(l->args) << std::endl;
		}
		return res;
	}

	std::string Log_to_string(std::shared_ptr<Log> l) const {
		std::stringstream res{};

		if (!l) return res.str();
		res << " ---------" << std::endl
			<< "| LogId: " << l->GetId() << std::endl
			<< "| Action: " << l->action << std::endl
			<< "| Description: " << l->Description.str() << std::endl
			<< "| Args: " << Misc::ArrToStr(l->args) << std::endl;

		return res.str();
	}

	std::list<std::shared_ptr<Log>> SearchLogs(Predicate<Log> predicate) const {
		std::list<std::shared_ptr<Log>> res{};
		for (std::shared_ptr<Log> l : db->Logs) {
			if (!predicate(l)) continue;
			res.push_back(l);
		}
		return res;
	}

	std::shared_ptr<Log> SearchLog(Predicate<Log> predicate) const {
		for (std::shared_ptr<Log> l : db->Logs) {
			if (!predicate(l)) continue;
			return l;
		}
		return nullptr;
	}
	friend class CLI;
};

