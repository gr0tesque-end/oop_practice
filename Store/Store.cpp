#include <iostream>
#include "CLI.h"
#include <list>

using namespace std;

int main() {
	RegisterClasses();

	Db* db{ Db::GetInstance() };
	
	CLI cli{};

	return 0;
}

/*
int main() {
	//^\s+(.+): (.+),
	RegisterClasses();
	Employee* admin = new Employee{ 0, "Admin", 0, 100, "admin" };
	Employee* emp1 = new Employee{ 1, "John", 0, 50, "1234" };
	Customer* cus1 = new Customer{ 1000, "Bill", 432.5f, new float[10] { 9.3f, 15.f }, 2, 0, "b123" };
	SubscribedCustomer* cus2 = SubscribedCustomer::Subscribe(std::move(Customer(2000, "Peter", 35.6f, new float[1], 1, 0, "p123")), SubPlans::Advanced);
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

	for (IObject* obj : list)
	{
		cout << obj->ToString().str() << ',';
	}


	//Serializer::Serialize(**(++std::make_move_iterator(list.begin())), "users_auth.json");
	/*auto res = Deserializer::Deserialize("db.json");
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
		generateObject(findAndRemoveRange(res, "Object")).release());*/

	/*{
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
		
	}
	return 0;
}
*/