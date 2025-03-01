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

class Log :
    public IIdentifiable
{
	ProductAction action;

	std::stringstream Description;

	// No need to call destructor on this
	std::forward_list<IIdentifiable*> Executioner;

	// Object(s) of Execution
	std::forward_list<IIdentifiable*> OoE;
public:
	Log(ProductAction a,
		std::forward_list<IIdentifiable*>&& execer,
		std::forward_list<IIdentifiable*>&& OoE) :
			IObject{ this }, IIdentifiable{}, action{ a },
			Executioner{ std::move(execer) },
			OoE{ std::move(OoE) } {

    }

	Log(const Log& l)
		: IObject{ this },
			IIdentifiable{ l.id },
			action{ l.action },
			Executioner{ l.Executioner },
			OoE{ l.OoE }{ 
		Description << l.Description.str();
	}

	Log(Log&& other) noexcept
		: IObject{ std::move(other) },
		IIdentifiable{ std::move(other) },
		action{ other.action },
		Description{ std::move(other.Description) },
		Executioner{ std::move(other.Executioner) },
		OoE{ std::move(other.OoE) }
	{
	}

	Log& operator=(Log&& other) noexcept {
		if (this != &other) {
			IObjectReference = other.IObjectReference;
			other.IObjectReference = nullptr;
			id = other.id;
			action = other.action;
			Description = std::move(other.Description);
			Executioner = std::move(other.Executioner);
			OoE = std::move(other.OoE);
		}
		return *this;
	}

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

	virtual const int GetId() const override { return id * -1000; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;
		
		ss << "{\n\tObject: \"" << "Log" << "\",\n"
			<< "\tId: \"" << GetId() << "\",\n"
			<< "\tAction: \"" << action << "\",\n"
			<< "\tDescription: \"\n\t\t" << Description.str() << "\n\t\",\n"
			<< "\tExecutioner(s): [ " << Misc::ArrToStr(Executioner) << " ],\n"
			<< "\tOoE(s): [ " << Misc::ArrToStr(OoE) << " ],\n"
			<< "}\n";
		return ss;
	};
};

