#pragma once
#include "IIdentifiable.h"
#include <iostream>
#include <sstream>
#include "Misc.h"
#include <forward_list>

// <do foo> Product
enum LogAction {
	Buy = 0,
	Return = 1,
	Restock = 2,
	Deposit = 3,
	Refund = 4
};

// 5.5
class Log final :
    public IIdentifiable
{
	LogAction action;

	std::stringstream Description;

	std::vector<std::string> args;

	int ExecutionerId;
	std::string CreatedBy;
public:
	const static std::string Type;

	Log(int id, 
		int a, 
		const std::vector<std::string> args, 
		const std::string& desc,
		int execerId,
		const std::string& CreatedBy):
		action{ (LogAction)a }, IIdentifiable{ id }, 
		args{args}, Description{desc}, ExecutionerId{execerId},
		CreatedBy{ CreatedBy } 
	{
		this->args.reserve(3);
	}

	Log(const std::string& id, 
		const std::string& a,
		const std::string& args,
		const std::string& desc,
		const std::string& execerId,
		const std::string& CreatedBy) :
		Log{ atoi(id.c_str()), atoi(a.c_str()), split(args, 255), desc, atoi(execerId.c_str()), CreatedBy }
	{
	}

	// 3.1
	Log(const Log& l) = delete;
	// 4.6
	Log(Log&& other) noexcept
		:
		IIdentifiable{ std::move(other) },
		action{ other.action },
		args{ std::move(other.args) },
		Description{ std::move(other.Description) },
		ExecutionerId{ std::move(other.ExecutionerId) }
	{
	}

	// 3.2 | 3.3 | 3.6 | 4.6
	Log& operator=(Log&& other) noexcept {
		if (this != &other) {
			id = other.id;
			action = other.action;
			args = std::move(other.args);
			Description = std::move(other.Description);
			ExecutionerId = std::move(other.ExecutionerId);
		}
		return *this;
	}

	// 3.7
	friend std::istream& operator>>(std::istream& is, Log& l) {
		std::string line;
		if (std::getline(is, line)) {
			l.Description.str(line);
			l.Description.clear();
		}
		return is;
	}

	Log* AddArg(std::string arg) { args.push_back(arg); return this; }

	Log& operator>>(const char* str) {
		Description << str;
		return *this;
	}
	Log& operator>>(int i) {
		Description << i;
		return *this;
	}

	std::string GetDescription() const {
		return Description.str();
	}

	virtual const int GetId() const override { return id; }

	bool operator==(std::string desc) const {
		return this->Description.str() == desc;
	}

	/*
		int id, 
		int a, 
		const std::vector<std::string>& args, 
		std::string& desc,
		int execerId
	*/

	virtual std::stringstream ToString() const override {
		std::stringstream ss;
		
		ss << "\n{\n\t\"Object\": \"" << "Log" << "\",\n"
			<< "\t\"Id\": \"" << GetId() << "\",\n"
			<< "\t\"Action\": \"" << action << "\",\n"
			<< "\t\"Arg(s)\": \"[ " << Misc::ArrToStr(args) << " ]\",\n"
			<< "\t\"Description\": \"" << Description.str() << "\",\n"
			<< "\t\"Executioner\": \"" << ExecutionerId << "\",\n"
			<< "\t\"CreatedBy\": \"" << CreatedBy << "\"\n"
			<< "},";
		return ss;
	};

	friend class Db;
	friend class Store;
};
const std::string Log::Type{ "Log" };