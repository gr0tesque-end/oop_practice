#pragma once
#include "IIdentifiable.h"
#include <iostream>
#include <sstream>
#include "Misc.h"
#include <forward_list>

// <do foo> Product
enum ProductAction {
	Buy, 
	Return,
	Restock
};

// 5.5
class Log final :
    public IIdentifiable
{
	ProductAction action;

	std::stringstream Description;

	std::vector<int> args;
	// 4.4

	// No need to call destructor on this
	std::forward_list<IIdentifiable*> Executioner;

	// Object(s) of Execution
	std::forward_list<IIdentifiable*> OoE;
public:
	// 4.5
	Log(ProductAction a,
		std::forward_list<IIdentifiable*>&& execer,
		std::forward_list<IIdentifiable*>&& OoE,
		std::vector<int>&& args) :
			IIdentifiable{}, action{ a },
			Executioner{ std::move(execer) },
			OoE{ std::move(OoE) }, args{ std::move(args) } {

    }

	Log(int id, int a, std::vector<int>& args, std::string& desc,
		std::vector<int>& execer,
		std::vector<int>& OoE):
		action{ (ProductAction)a }, Description{ desc }
	{
		this->id = id;
		for (int i: execer)
		{
			Executioner.push_front(new IIdentifiable(i));
		}
		for (int i : OoE)
		{
			this->OoE.push_front(new IIdentifiable(i));
		}
		for (int i : args)
		{
			this->args.push_back(i);
		}
	}

	// 3.1
	Log(const Log& l)
		: 	IIdentifiable{ l.id },
			action{ l.action },
			Executioner{ l.Executioner },
			OoE{ l.OoE }{ 
		Description << l.Description.str();
	}

	// 4.6
	Log(Log&& other) noexcept
		: 
		IIdentifiable{ std::move(other) },
		action{ other.action },
		Description{ std::move(other.Description) },
		Executioner{ std::move(other.Executioner) },
		OoE{ std::move(other.OoE) }
	{
	}

	// 3.2 | 3.3 | 3.6 | 4.6
	Log& operator=(Log&& other) noexcept {
		if (this != &other) {
			id = other.id;
			action = other.action;
			Description = std::move(other.Description);
			Executioner = std::move(other.Executioner);
			OoE = std::move(other.OoE);
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

	Log& operator>>(const char* str) {
		Description << str;
		return *this;
	}
	Log& operator>>(int i) {
		Description << i;
		return *this;
	}

	Log operator+(Log& rhs) {
		if (action != rhs.action) {
			throw std::invalid_argument("Cannot combine logs with different actions.");
		}

		Log result(*this);

		if (!rhs.Executioner.empty()) {
			result.Executioner.splice_after(result.Executioner.cbefore_begin(), rhs.Executioner);
		}

		if (!rhs.OoE.empty()) {
			result.OoE.splice_after(result.OoE.cbefore_begin(), rhs.OoE);
		}

		result.Description << std::endl << "\t\t" << rhs.Description.str();

		return result;
	}

	virtual const int GetId() const override { return - id - 10000; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;
		
		ss << "\n{\n\tObject: \"" << "Log" << "\",\n"
			<< "\tId: \"" << GetId() << "\",\n"
			<< "\tAction: \"" << action << "\",\n"
			<< "\tArg(s): \"[ " << Misc::ArrToStr(args) << " ]\",\n"
			<< "\tDescription: \"" << Description.str() << "\",\n"
			<< "\tExecutioner(s): \"[ " << Misc::ArrToStr(Executioner) << " ]\",\n"
			<< "\tOoE(s): \"[ " << Misc::ArrToStr(OoE) << " ]\"\n"
			<< "}";
		return ss;
	};
};

