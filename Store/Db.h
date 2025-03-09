﻿#pragma once
#include <string>
#include <list>
#include "Employee.h"
#include "Log.h"
#include "Regex-PresudoJSONParser.h"
#include "Product.h"
#include "SubscribedCustomer.h"
#include "ObjectFactory.h"

class Db final
{
	const std::string& File;

	Db(const std::string& file)
		: File{ file }
	{
		auto res = Deserializer::Deserialize(file);

		while (!res.empty()) {
			auto range = findAndRemoveRange(res, "Object");
			auto obj = generateObject(range);
			
			if (auto* item = dynamic_cast<Product*>(obj.get())) {
				Products.push_back(item);
			}
			else if (auto* item = dynamic_cast<Log*>(obj.get())) {
				Logs.push_back(item);
			}
			else if (auto* item = dynamic_cast<Customer*>(obj.get())) {
				Сustomers.push_back(item);
			}
			else if (auto* item = dynamic_cast<Employee*>(obj.get())) {
				Employees.push_back(item);
			}
			else {
				std::cerr << "Unknown object type!\n";
			}
			GlobalList.push_back(obj.release());
		}
	}
	static Db* db;
public:
	std::list<IObject*> GlobalList{};
	std::list<Employee*> Employees{};
	std::list<Customer*> Сustomers{};
	std::list<Product*> Products{};
	std::list<Log*> Logs{};

	Db(Db& other) = delete;
	Db(Db&& other) = delete;
	
	void operator=(const Db&) = delete;
	
	static Db* GetInstance(const std::string& file) {
		if (db == nullptr) {
			db = new Db(file);
		}
		return db;
	}

	void Flush() const {
		for (IObject* obj: GlobalList)
		{
			Serializer::Serialize(*obj, File);
		}
	}
};

Db* Db::db = nullptr;