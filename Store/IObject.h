#pragma once
#include <string>
#include <ostream>
#include <sstream>
#include <vector>

const int AuthorityEmployeeThreshhold{ 100 };

// 5.8
class IObject
{
public:
	// 5.7
	virtual std::stringstream ToString() const = 0;
	virtual ~IObject() {}

	bool operator==(const IObject* obj) const {
		return this->ToString().str() == obj->ToString().str();
	}
};

static std::vector<std::string> split(const std::string& s, char delim) {
	if (s.empty()) return {};
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

template<typename T>
static std::vector<T> split(const std::string& s, char delim, T(*convertion)(std::string&)) {
	std::vector<T> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(convertion(item));
	}

	return result;
}
