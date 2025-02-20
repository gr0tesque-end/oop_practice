#include <iostream>
#include "Employee.h"
#include "Product.h"
#include "Customer.h"

using namespace std;

static void doFoo(Employee& e, Product& p, Customer& c) {
	if (!p.IsInStock()) return;
	float pr = p.GetPrice();

	if (!c.Buy(pr)) return;
	p.ChangeQuantity(-1);

	e.ChangeSales(pr);
}

int main() {
	Employee* emp1 = new Employee{ "John" };
	Employee* emp2 = new Employee{ "Bob" };

	Product* products[5]{
		new Product{},
		new Product{ "Kidney", 100000 },
		new Product{ "Pasta", 5.23, 30 }
	};

	Customer c1{ "Peter", 841.2 };
	Customer c2{"Williams"};
	Customer c3{};

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

	return 0;
}
