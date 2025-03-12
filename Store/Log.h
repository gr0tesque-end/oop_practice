#pragma once
#include "IIdentifiable.h"
#include <iostream>
#include <sstream>
#include "Misc.h"
#include <forward_list>

// <do foo> Product
enum ProductAction {
	Buy = 0, 
	Return = 1,
	Restock = 2
};

// 5.5
class Log final :
    public IIdentifiable
{
	ProductAction action;

	std::stringstream Description;

	std::vector<std::string> args;

	int ExecutionerId;
public:
	Log(int id, 
		int a, 
		const std::vector<std::string> args, 
		const std::string& desc,
		int execerId):
		action{ (ProductAction)a }, args{args}, Description{desc}, ExecutionerId{ id }
	{
		this->args.reserve(3);
		this->id = id;
	}

	Log(const std::string& id, 
		const std::string& a,
		const std::string& args,
		const std::string& desc,
		const std::string& execerId) :
		Log{ atoi(id.c_str()), atoi(a.c_str()), split(args, 255), desc, atoi(execerId.c_str()) }
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

	void AddArg(std::string arg) { args.push_back(arg); }

	Log& operator>>(const char* str) {
		Description << str;
		return *this;
	}
	Log& operator>>(int i) {
		Description << i;
		return *this;
	}

	virtual const int GetId() const override { return id; }


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
			<< "\t\"Executioner\": \"" << ExecutionerId << "\"\n"
			<< "},";
		return ss;
	};
};

