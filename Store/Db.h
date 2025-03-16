#pragma once
#include <string>
#include <list>
#include "Regex-PresudoJSONParser.h"
#include "ObjectFactory.h"

const unsigned int FileCount{ 4 };

class Db;

template<typename T>
concept DbStorable = std::is_base_of_v<IIdentifiable, T>;

template<typename T>
using Predicate = std::function<bool(std::shared_ptr<T>)>;

class Db final
{
	template<DbStorable T>
	std::list<std::shared_ptr<T>> GetContainer() const {
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

	template<DbStorable ChildT, class BaseT>
	std::list<std::shared_ptr<BaseT>> CastContainerToBase() const {
		std::list<std::shared_ptr<BaseT>> res{};
		for (const auto& elem : GetContainer<ChildT>()) {
			res.push_back(elem);
		}
		return res;
	}

	std::list<std::shared_ptr<IObject>> GetContainer(const char* Type) const {
		switch (Type[0])
		{
		case 'L':
			return CastContainerToBase<Log, IObject>();
			break;
		case 'E':
			return CastContainerToBase<Employee, IObject>();
			break;
		case 'C':
			return CastContainerToBase<Customer, IObject>();
			break;
		case 'P':
			return CastContainerToBase<Product, IObject>();
			break;
		default:
			break;
		}
		return {
			nullptr
		};
	}

	template<DbStorable T>
	int GetContainerIndex() {
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

	template<DbStorable T>
	Db* Flush(bool sort = false) {
		auto cont = GetContainer<T>();

		if (sort) {
			cont.sort([](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
				return a->GetId() < b->GetId();
				});
		}

		Serializer::Serialize(*cont.front(), files[GetContainerIndex<T>()], std::ios::out);
		for (int i{}; auto & o: cont)
		{
			if (i++ == 0) continue;
			Serializer::Serialize(*o, files[GetContainerIndex<T>()], std::ios::app);
		}
		return this;
	}

	template<DbStorable T>
	std::shared_ptr<T> Search(Predicate<T> pred) const {
		for (const auto& ptr : GetContainer<T>()) {
			if (pred(ptr)) return ptr;
		}
	}

	std::list<std::shared_ptr<IObject>> SearchAll(const char* Type, Predicate<IObject> pred) const {
		std::list<std::shared_ptr<IObject>> res{ };
		for (auto& ptr : GetContainer(Type)) {
			if (pred(ptr)) res.push_back(ptr);
		}
		return res;
	}

	friend class Authenticator;
	friend class Store;
};

Db* Db::db = nullptr;