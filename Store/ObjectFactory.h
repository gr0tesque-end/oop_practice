#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "Customer.h"
#include "Employee.h"
#include "Log.h"


class ObjectFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IIdentifiable>(const std::string&)>;

    static void Register(const std::string& name, CreatorFunc creator) {
        GetRegistry()[name] = std::move(creator);
    }

    static std::unique_ptr<IIdentifiable> Create(const std::string& name, const std::string& param) {
        auto& registry = GetRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second(param);
        }
        return nullptr;
    }

private:
    static std::unordered_map<std::string, CreatorFunc>& GetRegistry() {
        static std::unordered_map<std::string, CreatorFunc> registry;
        return registry;
    }
};



static std::unique_ptr<IIdentifiable> generateObject(std::list<std::pair<std::string, std::string>> objStr) {
	std::string str{};
	for (auto p : objStr)
	{
		str += p.second + '|';
	}

	return ObjectFactory::Create(objStr.front().second, str);
}

static void RegisterClasses() {
	static bool c;
	if (c) return;
	c = 0;
	ObjectFactory::Register("Customer",
		[](const std::string& param) -> std::unique_ptr<IIdentifiable> {
			auto params = split(param, '|');

			return std::make_unique<Customer>(
				params[1],
				params[2],
				params[3],
				params[4],
				params[5]
			);
		});
	//ObjectFactory::Register("SubscribedCustomer",
	//	[](const std::string& param) {
	//		return std::make_unique<SubscribedCustomer>(
	//			std::move(*dynamic_cast<Customer*>(ObjectFactory::Create("Customer", param).release())),
	//			atoi(split(param, '|')[7]
	//		);
	//	});
	ObjectFactory::Register("Employee",
		[](const std::string& param) -> std::unique_ptr<IIdentifiable> {
			auto params = split(param, '|');
			// Id, Name, Sales, pass
			return std::make_unique<Employee>(
				params[1],
				params[2],
				params[3],
				params[4],
				params[5]
			);
		});
	ObjectFactory::Register("Product",
		[](const std::string& param) -> std::unique_ptr<IIdentifiable> {
			auto params = split(param, '|');
			// Id, Name, Sales
			return std::make_unique<Product>(
				params[1],
				params[2],
				params[3],
				params[4]
			);

		});

	/*
		int id, 
		int a, 
		const std::vector<std::string>& args, 
		std::string& desc,
		int execerId
	*/
	ObjectFactory::Register("Log",
		[](const std::string& param) -> std::unique_ptr<IIdentifiable> {
			auto params = split(param, '|');

			auto s = split(params[3], '"');

			std::string args_parsed{};

			for (int i{}; auto& e : s) {
				if (i++ % 2 == 0) continue;
				args_parsed += e + char(255);
			}
			return std::make_unique<Log>(
				params[1],
				params[2],
				args_parsed,
				params[4],
				params[5]
			);
		});
	c = 1;
}
