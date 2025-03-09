#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "IObject.h"

class ObjectFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IObject>(const std::string&)>;

    static void Register(const std::string& name, CreatorFunc creator) {
        GetRegistry()[name] = std::move(creator);
    }

    static std::unique_ptr<IObject> Create(const std::string& name, const std::string& param) {
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

static std::unique_ptr<IObject> generateObject(std::list<std::pair<std::string, std::string>> objStr) {
	std::string str{};
	for (auto p : objStr)
	{
		str += p.second + '|';
	}

	return ObjectFactory::Create(objStr.front().second, str);
}

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

static void RegisterClasses() {
	static bool c;
	if (c) return;
	c = 0;
	ObjectFactory::Register("Customer",
		[](const std::string& param) {
			auto params = split(param, '|');
			auto history = split<float>(params[4], ',', [](std::string& s)->float {
				return atof(s.c_str());
				});

			// Id, Name, Balance, history, size
			return std::make_unique<Customer>(
				atoi(params[1].c_str()),
				params[2],
				atof(params[3].c_str()),
				history.data(),
				history.size(),
				atoi(params[5].c_str()),
				params[6]
			);
		});
	ObjectFactory::Register("SubscribedCustomer",
		[](const std::string& param) {
			return std::make_unique<SubscribedCustomer>(
				std::move(*dynamic_cast<Customer*>(ObjectFactory::Create("Customer", param).release())),
				atoi(split(param, '|')[7].c_str())
			);
		});
	ObjectFactory::Register("Employee",
		[](const std::string& param) {
			auto params = split(param, '|');
			// Id, Name, Sales, pass
			return std::make_unique<Employee>(
				atoi(params[1].c_str()),
				params[2],
				atof(params[3].c_str()),
				atoi(params[4].c_str()),
				params[5]
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
		[](const std::string& param)-> std::unique_ptr<IObject> {
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
