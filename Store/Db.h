#pragma once
#include <string>
#include <list>
#include "Employee.h"
#include "Log.h"
#include "Regex-PresudoJSONParser.h"
#include "Product.h"
#include "SubscribedCustomer.h"
#include "ObjectFactory.h"

const unsigned int FileCount{ 2 };

class Db;

template<typename T>
concept isStoredInDb = std::is_base_of_v<IIdentifiable, T>;

class Db final
{
	Db(const std::string& file)
	{
		files[0] = file;
		auto rawData = Deserializer::Deserialize(file);

		while (!rawData.empty()) {
			auto range = findAndRemoveRange(rawData, "Object");
			auto obj = generateObject(range);

			if (auto* item = dynamic_cast<Product*>(obj.get())) {
				Products.push_back(item);
			}
			else if (auto* item = dynamic_cast<Log*>(obj.get())) {
				Logs.push_back(item);
			}
			else if (auto* item = dynamic_cast<Customer*>(obj.get())) {
				Customers.push_back(item);
			}
			else if (auto* item = dynamic_cast<Employee*>(obj.get())) {
				Employees.push_back(item);
			}
			else {
				std::cerr << "Unknown object type!\n";
			}

			GlobalList.push_back(dynamic_cast<IIdentifiable*>(obj.release()));
		}
	}
	static Db* db;

public:
	std::string files[FileCount]{};

	std::list<IIdentifiable*> GlobalList{};
	std::list<Employee*> Employees{};
	std::list<Customer*> Customers{};
	std::list<Product*> Products{};
	std::list<Log*> Logs{};

	Db(Db& other) = delete;
	Db(Db&& other) = delete;

	void operator=(const Db&) = delete;

	static Db* GetInstance(const std::string& file = "Data/db.json") {
		if (db == nullptr) {
			db = new Db(file);
		}
		return db;
	}

	void Flush() const {
		for (IObject* obj : GlobalList)
		{
			Serializer::Serialize(*obj, files[0]);
		}
	}

	/*template<isStoredInDb T>
	T* Find(int id) {
		for (auto p : GlobalList) {
			if (p->GetId() == id) return dynamic_cast<T*>(p);
		}
	}*/

	Product* SearchProduct(int id) {
		for (auto p : Products) {
			if (p->GetId() == id) return p;
		}
	}
};

Db* Db::db = nullptr;