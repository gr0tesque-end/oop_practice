#include <iostream>
#include "Employee.h"
#include "Product.h"
#include "Customer.h"
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

int main() {
	Employee* emp1 = new Employee{ "John" };
	IObject* emp2 = emp1;

	Customer c1{ "Peter", 841.2f };
	Customer c2{"Williams"};
	Customer c3{};

	/*IObject o{};
	IIdentifiable iid{};
	INameable iN{};*/

	std::cout << emp1->GetSales() << endl; // 0.0
	std::cout << (*emp1).ChangeSales(841.60f)
				   .GetSales() 
		 << endl;
	
	std::cout << emp2->ToString().str() << endl; // John with 841.60 in sales

	std::cout << "Creating a Log...\n";

	auto p = new Product{ "Pasta", 5.23f, 30 };

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
	cout << l << endl << *p << endl;

	int RestockValue = 5;

	p->ChangeQuantity(RestockValue);

	Log l2{
		ProductAction::Restock,
		{
			emp1
		},
		{
			p
		}
	};
	l2 >> "Restock: " >> RestockValue;
	cout << l2 << endl;

	cout << *p << endl << l + l2;
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
