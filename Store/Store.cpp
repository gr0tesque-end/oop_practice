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

static void doFoo(Employee& e, Product& p, Customer& c) {
	if (!p.IsInStock()) return;
	float pr = p.GetPrice();

	if (!c.Buy(pr)) return;
	p.ChangeQuantity(-1);

	e.ChangeSales(pr);
}

int IIdentifiable::Id = 0;

//class ConcreteLog : public Log {};

static void RegisterClasses() {
	static bool c;
	if (c) return;
	c = 0;
	ObjectFactory::Register("Customer", []() { return std::make_unique<Customer>(); });
	ObjectFactory::Register("SubscribedCustomer", []() { return std::make_unique<Customer>(); });
	ObjectFactory::Register("Employee", []() { return std::make_unique<Employee>(); });
	ObjectFactory::Register("Product", []() { return std::make_unique<Product>(); });
	ObjectFactory::Register("Log", []() { return std::make_unique<Log>(); });
	c = 1;
}

int main() {
	//^\s+(.+): (.+),
	RegisterClasses();
	Employee* emp1 = new Employee{ "John" };
	Customer* cus1 = new Customer{ "Bill", 432.5f };
	SubscribedCustomer* cus2 = SubscribedCustomer::Subscribe(std::move(Customer("Peter", 35.6f)), SubPlans::Advanced);
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
	auto res = Deserializer::Deserialize("users_auth.json");
	findAndRemoveRange(res, "Object");
	findAndRemoveRange(res, "Object");
	findAndRemoveRange(res, "Object");


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
