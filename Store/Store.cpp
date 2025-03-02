#include <iostream>
#include "Employee.h"
#include "Product.h"
#include "Customer.h"
#include "SubscribedCustomer.h"
#include "Log.h"

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

int main() {
	// 5.3
	Employee* emp1 = new Employee{ "John" };
	IObject* emp2 = emp1;
	std::cout << emp2->ToString().str() << endl; // John; Object Employee

	Customer* c1 = new Customer{ "Peter", 841.2f };
	Customer c2{"Williams"};
	Customer c3{};

	// 3.4
	const Product pConst("Shoes", 10.43, 6);
	cout << pConst.ToString().str() << endl;

	SubscribedCustomer* c = SubscribedCustomer::Subscribe(*c1, SubPlans::Advanced);
	IIdentifiable* cPtr = dynamic_cast<IIdentifiable*>(c);

	/*IObject o{};
	IIdentifiable iid{};
	INameable iN{};*/

	std::cout << "Creating a Log...\n";

	Product* p = new Product{ "Pasta", 5.23f, 30 };
	// 3.6
	++(*p); // Works
	// ++p // Carefull!! incrementing a pointer

	Log l{
		ProductAction::Restock,
		{
			dynamic_cast<Employee*>(emp2)
		},
		{
			p,
			new Product{ "Kidney", 100000 }
		}
	};

	cout << "Enter a description: \n> ";
	cin >> l;
	cout << l.ToString().str() << endl << p->ToString().str() << endl;

	int RestockValue = 5;

	p->ChangeQuantity(RestockValue);

	Log l2{
		ProductAction::Restock,
		{
			emp1
		},
		{
			p, cPtr
		}
	};
	l2 >> "Restock: " >> RestockValue;
	cout << l2.ToString().str() << endl;

	cout << p->ToString().str() << endl << (l + l2).ToString().str();
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
