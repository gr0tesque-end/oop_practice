#include <iostream>
#include "Employee.h"
#include "Product.h"
#include "Customer.h"
#include "SubscribedCustomer.h"
#include "Log.h"
#include <list>
#include "Regex-PresudoJSONParser.h"
#include "ObjectFactory.h"

using namespace std;

static std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

template<typename T>
static std::vector<T> split(const std::string& s, char delim, T(*convertion)(std::string&)) {
	std::vector<T> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(convertion(item));
	}

	return result;
}

int IIdentifiable::Id = 0;

//class ConcreteLog : public Log {};

static void RegisterClasses() {
	static bool c;
	if (c) return;
	c = 0;
	ObjectFactory::Register("Customer", 
		[](const std::string& param) {
			auto params = split(param, '|');
			auto history = split<float>(params.back(), ',', [](std::string& s)->float {
				return atof(s.c_str());
				});

			// Id, Name, Balance, history, size
			return std::make_unique<Customer>(
				atoi(params[1].c_str()),
				params[2],
				atof(params[3].c_str()),
				history.data(),
				history.size()
			);
		});
	ObjectFactory::Register("SubscribedCustomer", 
		[](const std::string& param) {
			return std::make_unique<SubscribedCustomer>(
				std::move(*dynamic_cast<Customer*>(ObjectFactory::Create("Customer", param).release())),
				atoi(split(param, '|')[4].c_str())
			);
		});
	ObjectFactory::Register("Employee", 
		[](const std::string& param) {
			auto params = split(param, '|');
			// Id, Name, Sales
			return std::make_unique<Employee>(
				atoi(params[1].c_str()),
				params[2],
				atof(params[3].c_str())
			);
		});
	ObjectFactory::Register("Product", 
		[](const std::string& param) { 
			auto params = split(param, '|');
			// Id, Name, Sales
			return std::make_unique<Product>(
				atoi(params[1].c_str()),
				params[2],
				atof(params[3].c_str()),
				atoi(params[4].c_str())
			);

		});
	ObjectFactory::Register("Log", 
		[](const std::string& param)-> std::unique_ptr<IObject>{
			auto params = split(param, '|');
			auto args = split<int>(params[3], ',', [](std::string& s) {
				return atoi(s.c_str());
				});
			auto ex = split<int>(params[5], ',', [](std::string& s) {
				return atoi(s.c_str());
				});
			auto ooe = split<int>(params[6], ',', [](std::string& s) {
				return atoi(s.c_str());
				});

			// Id, Action, Arg(s), Description, Executioner(s), OoE(s)
			return std::make_unique<Log>(
				atoi(params[1].c_str()),
				atoi(params[2].c_str()),
				args,
				params[4],
				ex,
				ooe
			);
		});
	c = 1;
}

static unique_ptr<IObject> generateObject(std::list<std::pair<std::string, std::string>> objStr) {
	string str{};
	for (auto p : objStr)
	{
		str += p.second + '|';
	}
	
	return ObjectFactory::Create(objStr.front().second, str);
}

int main() {
	//^\s+(.+): (.+),
	RegisterClasses();
	Employee* emp1 = new Employee{ "John" };
	Customer* cus1 = new Customer{ "Bill", 432.5f };
	SubscribedCustomer* cus2 = SubscribedCustomer::Subscribe(std::move(Customer("Peter", 35.6f)), SubPlans::Advanced);
	cus2->Buy(3);
	Product* prod1 = new Product{ "Chair", 5.34f, 100 };
	Product* prod2 = new Product{ "Table", 11.87f, 100 };
	Log* l1 = new Log{
		ProductAction::Buy,
		{
			cus1
		},
		{
			prod1
		},
		{
			3
		}
	};
	Log* l2 = new Log{
		ProductAction::Buy,
		{
			cus2
		},
		{
			prod1, prod2
		},
		{ 6, 1 }
	};

	std::list<IObject*> list{ 
		emp1, cus1, cus2, prod1, prod2, l1, l2
	};

	for (IObject* obj: list)
	{
		cout << obj->ToString().str() << ',';
	}
	//Serializer::Serialize(**(++std::make_move_iterator(list.begin())), "users_auth.json");
	auto res = Deserializer::Deserialize("db.json");
	findAndRemoveRange(res, "Object");
	auto obj1 = dynamic_cast<Customer*>(
		generateObject(findAndRemoveRange(res, "Object")).release());
	auto obj2 = dynamic_cast<SubscribedCustomer*>(
		generateObject(findAndRemoveRange(res, "Object")).release());
	auto obj3 = dynamic_cast<Product*>(
		generateObject(findAndRemoveRange(res, "Object")).release());
	findAndRemoveRange(res, "Object");
	auto obj4 = dynamic_cast<Log*>(
		generateObject(findAndRemoveRange(res, "Object")).release());
	auto obj5 = dynamic_cast<Log*>(
		generateObject(findAndRemoveRange(res, "Object")).release());


	// 5.3
	//IObject* emp2 = emp1;
	//std::cout << emp2->ToString().str() << endl; // John; Object Employee

	//Customer* c1 = new Customer{ "Peter", 841.2f };
	//Customer c2{"Williams"};
	//Customer c3{};

	//// 3.4
	//const Product pConst("Shoes", 10.43, 6);
	//cout << pConst.ToString().str() << endl;

	//SubscribedCustomer* c = SubscribedCustomer::Subscribe(*c1, SubPlans::Advanced);
	//IIdentifiable* cPtr = dynamic_cast<IIdentifiable*>(c);

	///*IObject o{};
	//IIdentifiable iid{};
	//INameable iN{};*/

	//std::cout << "Creating a Log...\n";

	//Product* p = new Product{ "Pasta", 5.23f, 30 };
	//// 3.6
	//++(*p); // Works
	//// ++p // Carefull!! incrementing a pointer

	//Log l{
	//	ProductAction::Restock,
	//	{
	//		dynamic_cast<Employee*>(emp2)
	//	},
	//	{
	//		p,
	//		new Product{ "Kidney", 100000 }
	//	}
	//};

	//cout << "Enter a description: \n> ";
	//cin >> l;
	//cout << l.ToString().str() << endl << p->ToString().str() << endl;

	//int RestockValue = 5;

	//p->ChangeQuantity(RestockValue);

	//Log l2{
	//	ProductAction::Restock,
	//	{
	//		emp1
	//	},
	//	{
	//		p, cPtr
	//	}
	//};
	//l2 >> "Restock: " >> RestockValue;
	//cout << l2.ToString().str() << endl;

	//cout << p->ToString().str() << endl << (l + l2).ToString().str();
/*
	doFoo(*emp1, *products[0], c1);
	doFoo(*emp1, *products[0], c2);
	doFoo(*emp1, *products[0], c3);

	doFoo(*emp1, *products[1], c1);

	doFoo(*emp1, *products[2], c1);

	doFoo(*emp2, *products[0], c1);

	doFoo(*emp2, *products[1], c1);

	auto v = c1.GetHistory();

	for (int i = 0; i < c1.GetIndex(); i++)
	{
		cout << v[i] << endl;
	}
	*/
	return 0;
}
