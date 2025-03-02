#pragma once
#include <map>
#include "Customer.h"

extern // Time in days
const std::map<SubPlans, int> Plans{
	{ SubPlans::Basic, 28 },
	{ SubPlans::Advanced, 44 },
	{ SubPlans::Premium, 84 }
};

class SubscribedCustomer :
	public Customer
{
	int SubscribtionTime;

public:
	SubscribedCustomer(const SubscribedCustomer& c) :
		Customer(c), SubscribtionTime{ c.SubscribtionTime } {
	}

	SubscribedCustomer(Customer& c, SubPlans plan) :
		SubscribtionTime{ Plans.at(plan) }, Customer(c) {

	}

	SubscribedCustomer(SubscribedCustomer&& c) noexcept :
		Customer(c), SubscribtionTime{ c.SubscribtionTime }
	{
		c.SubscribtionTime = 0;
	}

	// 5.2 | 5.5
	virtual bool Buy(const float& price) override final {
		bool res = Customer::Buy(price);
		if (res) _balance += 0.05f * price;
		return res;
	}

	static _NODISCARD SubscribedCustomer* Subscribe(Customer& c, SubPlans plan) {
		if (c.Buy((int)plan)) {
			return new SubscribedCustomer(c, plan);
		}
		return nullptr;
	}
};

