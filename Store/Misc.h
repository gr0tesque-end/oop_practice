#pragma once
#include <string>
#include <sstream>
#include <forward_list>

#include <cmath>

bool areEqual(float a, float b, int decimalPlaces = 2) {
	float multiplier = std::pow(10, decimalPlaces);
	float roundedA = std::round(a * multiplier) / multiplier;
	float roundedB = std::round(b * multiplier) / multiplier;

	return roundedA == roundedB;
}

bool isLess(float a, float b, int decimalPlaces = 2) {
	float multiplier = std::pow(10, decimalPlaces);
	float roundedA = std::round(a * multiplier) / multiplier;
	float roundedB = std::round(b * multiplier) / multiplier;

	return roundedA > roundedB;
}
bool isBigger(float a, float b, int decimalPlaces = 2) {
	float multiplier = std::pow(10, decimalPlaces);
	float roundedA = std::round(a * multiplier) / multiplier;
	float roundedB = std::round(b * multiplier) / multiplier;

	return roundedA < roundedB;
}

class Misc
{
public:
	Misc() = delete;

	template<typename T>
	static std::string ArrToStr(T arr, int size, char sep = ',') {
		std::stringstream ss;

		for (int i = 0; i < size; ++i)
		{
			ss << * (arr + i);
			if (i < size - 1) ss << sep;
		}

		return ss.str();
	}

	static std::string ArrToStr(const std::forward_list<IIdentifiable*>& arr) {
		std::stringstream ss;
		bool first = true;

		for (IIdentifiable* obj : arr) {
			if (!first) ss << ',';
			first = false;
			ss << obj->GetId();
		}

		return ss.str();
	}

	static std::string ArrToStr(const std::vector<int>& arr) {
		std::stringstream ss;
		bool first = true;

		for (int obj : arr) {
			if (!first) ss << ',';
			first = false;
			ss << std::to_string(obj);
		}

		return ss.str();
	}

	static std::string ArrToStr(const std::vector<std::string>& arr) {
		std::stringstream ss;
		bool first = true;

		for (auto& obj : arr) {
			if (!first) ss << ',';
			first = false;
			ss << '"' << obj << '"';
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

