#pragma once
#include <string>
#include <list>
#include "Regex-PresudoJSONParser.h"
#include "ObjectFactory.h"

const unsigned int FileCount{ 4 };

class Db;

template<typename T>
concept DbStorable = std::is_base_of_v<IIdentifiable, T>;

enum Objs {
	Logs = 0,
	Employees = 1,
	Products = 2,
	Customers = 3
};

class Db final
{
	/*template<DbStorable T>
	static std::shared_ptr<T> GenerateObject(const std::list<std::pair<std::string, std::string>>* obj)
	{
		IIdentifiable* obj = generateObject(*obj);
		return std::make_shared<T>(dynamic_cast<T*>(obj));
	}*/

	template<DbStorable T>
	std::list<std::shared_ptr<T>>& GetContainer() {
		if constexpr (std::is_same_v<T, Log>) {
			return Logs;
		}
		else if constexpr (std::is_same_v<T, Employee>)
		{
			return Employees;
		}
		else if constexpr (std::is_same_v<T, Product>) {
			return Products;
		}
		else if constexpr (std::is_same_v<T, Customer>)
		{
			return Customers;
		}
	}

	template<DbStorable T>
	int GetIndex() {
		if constexpr (std::is_same_v<T, Log>) {
			return 0;
		}
		else if constexpr (std::is_same_v<T, Employee>)
		{
			return 1;
		}
		else if constexpr (std::is_same_v<T, Product>) {
			return 2;
		}
		else if constexpr (std::is_same_v<T, Customer>)
		{
			return 3;
		}
	}

	// Logs > Employees > Products > Customers
	Db(const std::vector<std::string> files)
	{
		LatestIds.resize(FileCount);

		for (int i{}; i < FileCount; ++i) {
			this->files[i] = files[i];

			auto rawData = Deserializer::Deserialize(this->files[i]);

			while (!rawData.empty()) {
				auto range = findAndRemoveRange(rawData, "Object");

				auto obj = generateObject(range).release();

				switch (i)
				{
				case 0: 
					Logs.push_back(std::shared_ptr<Log>(dynamic_cast<Log*>(obj)));
					break;
				case 1:
					Employees.push_back(std::shared_ptr<Employee>(dynamic_cast<Employee*>(obj)));
					break;
				case 2:
					Products.push_back(std::shared_ptr<Product>(dynamic_cast<Product*>(obj)));
					break;
				case 3:
					Customers.push_back(std::shared_ptr<Customer>(dynamic_cast<Customer*>(obj)));
					break;
				default:
					break;
				}
				int id = atoi(range.begin()._Ptr->_Next->_Myval.second.c_str());
				LatestIds[i] = id;
			}
		}

			/*if (auto* item = dynamic_cast<Product*>(obj.get())) {
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
			}*/

			//GlobalList.push_back(&Employees);
		
	}
	static Db* db;

	std::list<std::shared_ptr<Employee>> Employees{};
	std::list<std::shared_ptr<Customer>> Customers{};
	std::list<std::shared_ptr<Product>> Products{};
	std::list<std::shared_ptr<Log>> Logs{};

	std::vector<int> LatestIds{};
public:
	std::string files[FileCount]{};

	template<DbStorable T>
	Db* push_back(std::shared_ptr<T> obj) {

		if constexpr (std::is_same_v<T, Log>) {
			if (obj->id < 0) obj->id = ++LatestIds[0];
			Logs.push_back(obj);
		}
		else if constexpr (std::is_same_v<T, Employee>)
		{
			if (obj->id < 0) obj->id = ++LatestIds[1];
			Employees.push_back(obj);
		}
		else if constexpr (std::is_same_v<T, Product>) {
			if (obj->id < 0) obj->id = ++LatestIds[2];
			Products.push_back(obj);
		}
		else if constexpr (std::is_same_v<T, Customer>)
		{
			if (obj->id < 0) obj->id = ++LatestIds[3];
			Customers.push_back(obj);
		}
		return this;
	}

	Db(Db& other) = delete;
	Db(Db&& other) = delete;

	void operator=(const Db&) = delete;

	static Db* GetInstance(const std::vector<std::string> files = {
							"Data/Logs.json",
							"Data/Employees.json",
							"Data/Products.json",
							"Data/Customers.json" })
	{
		if (db == nullptr) {
			db = new Db(files);
		}
		return db;
	}

	// Sorts by Id before flush
	template<DbStorable T>
	void Flush() {
		auto& cont = GetContainer<T>();
		cont.sort([](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
			return a->GetId() < b->GetId();
			});
		Serializer::Serialize(*cont.front(), files[GetIndex<T>()], std::ios::out);
		for (int i{}; auto& o: cont)
		{
			if (i++ == 0) continue;
			Serializer::Serialize(*o, files[GetIndex<T>()], std::ios::app);
		}
	}

	/*template<isStoredInDb T>
	T* Find(int id) {
		for (auto p : GlobalList) {
			if (p->GetId() == id) return dynamic_cast<T*>(p);
		}
	}*/

	std::shared_ptr<Product> SearchProduct(int id) {
		for (auto p : Products) {
			if (p->GetId() == id) return p;
		}
		return nullptr;
	}

	friend class Authenticator;
	friend class Store;
};

Db* Db::db = nullptr;