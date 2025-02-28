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
			Executioner{ execer },
			OoE{ OoE } {

    }

	friend std::istream& operator>>(std::istream& is, Log& l) {
		std::string line;
		if (std::getline(is, line)) {
			l.Description.str(line);
			l.Description.clear();
		}
		return is;
	}

	virtual const int GetId() const override { return Id * -1000; }

	virtual std::stringstream ToString() const override {
		std::stringstream ss;
		
		ss << "{\n\tObject: \"" << "Log" << "\",\n"
			<< "\tId: \"" << Id << "\",\n"
			<< "\tAction: \"" << action << "\",\n"
			<< "\tExecutioner(s): [ " << Misc::ArrToStr(Executioner) << " ],\n"
			<< "\tOoE(s): [ " << Misc::ArrToStr(OoE) << " ],\n"
			<< "}\n";
		return ss;
	};
};

