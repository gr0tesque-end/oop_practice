#pragma once
#include <iostream>
#include <regex>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include "IObject.h"

// current kvpa = ^\s*"(\w+\(?s?\)?)": "\[?([^\n\r\t\0)]*)\]?"

const std::string kvp{ "^\\s+(\\w+): \"(.*)\"" };
const std::string kvpl{ R"(^\s+(\w+\(s\)): "\[ (.*) \]")" };
const std::string kvpa{ "^\\s*\"(\\w+\\(?s?\\)?)\": \"\\[?([^\\n\\r\\t\\0)]*)\\]?\""
    /*"^\\s*\"(\\w+\\(?s?\\)?)\": \"\\[?\\s*([\\w\\.\\-\\,]*(?:\\s+[\\w\\.\\-\\,]+)*)\\s*\\]?\""*/
    /*"^\\s+(\\w+\\(?s?\\)?): \"(.*)\""*/ };

// ((?<=")([^(",")]*)(?=")) to extract elements of list<string>

// kvpa raw = ^\s*"(\w+\(?s?\)?)": "\[?\s*"?([\w\.\-\,>]*(?:\s+[\w\.\-\,>]+)*)"?\s*\]?"

std::list<std::pair<std::string, std::string>> extractKeyValuePairs(
    const std::string& input,
    std::string patternStr) {
    std::list<std::pair<std::string, std::string>> result;
    std::regex pattern{ patternStr, std::regex_constants::ECMAScript };
    std::smatch match;

    auto begin = input.cbegin();
    auto end = input.cend();
    while (std::regex_search(begin, end, match, pattern)) {
        result.push_back(std::make_pair(match[1].str(),match[2].str()));
        begin = match.suffix().first;
    }
    return result;
}



/*std::list<std::pair<std::string, std::string>> findRange(
    const std::list<std::pair<std::string, std::string>>& list, const std::string& target) {
    std::list<std::pair<std::string, std::string>> result;
    auto it = list.begin();
    auto end = list.end();
    bool inRange = false;

    for (; it != end; ++it) {
        if (it->first == target) {
            if (inRange) break;
            inRange = true;
        }
        if (inRange) {
            result.push_back(*it);
        }
    }

    return result;
}*/

std::list<std::pair<std::string, std::string>> findAndRemoveRange(
    std::list<std::pair<std::string, std::string>>& lst,
    const std::string& target)
{
    std::list<std::pair<std::string, std::string>> extracted;
    auto it = lst.begin();

   
    while (it != lst.end() && it->first != target) {
        ++it;
    }

    if (it == lst.end()) return {}; 

    auto rangeStart = it;
    extracted.push_back(*it);
    ++it;

    while (it != lst.end() && it->first != target) {
        extracted.push_back(*it);
        ++it;
    }

    lst.erase(rangeStart, it);

    return extracted;
}

class Serializer {
public:
    static void Serialize(const IObject& obj, const std::string& filename, std::ios_base::openmode mode) {
        std::ofstream file(filename, mode);
        if (file.is_open()) {
            file << obj.ToString().str();
            file.close();
        }
    }
};

struct DeserializationResult {
    std::list<std::pair<std::string, std::string>> Group1;
    std::list<std::pair<std::string, std::string>> Group2;
};

class Deserializer {
public:
    static std::list<std::pair<std::string, std::string>> Deserialize(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        DeserializationResult res;

        std::string content = buffer.str();

        return extractKeyValuePairs(content, kvpa);
    }
};

