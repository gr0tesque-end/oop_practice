#pragma once
#include <string>
#include <sstream>
#include <forward_list>

class Misc
{
public:
	Misc() = delete;

	template<typename T>
	static std::string ArrToStr(T arr, int size, char sep = ',') {
		std::stringstream ss;

		for (int i = 0; i < size;)
		{
			ss << * (arr + i) << (++i == size) ? " " + sep : " ";
		}

		return ss.str();
	}

	static std::string ArrToStr(const std::forward_list<IIdentifiable*>& arr) {
		std::stringstream ss;
		bool first = true;

		for (IIdentifiable* obj : arr) {
			if (!first) ss << ", ";
			first = false;
			ss << obj->GetId();
		}

		return ss.str();
	}

	/*static std::string ArrToStr(IObject* arr, int size, char sep = ',') {
		std::stringstream ss;

		for (int i = 0; i < size;)
		{
			ss << (arr + i)->ToString().str() << (++i == size) ? " " + sep : " ";
		}

		return ss.str();
	}*/
};

