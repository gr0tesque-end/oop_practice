#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "IObject.h"

class ObjectFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IObject>(const std::string&)>;

    static void Register(const std::string& name, CreatorFunc creator) {
        GetRegistry()[name] = std::move(creator);
    }

    static std::unique_ptr<IObject> Create(const std::string& name, const std::string& param) {
        auto& registry = GetRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second(param);
        }
        return nullptr;
    }

private:
    static std::unordered_map<std::string, CreatorFunc>& GetRegistry() {
        static std::unordered_map<std::string, CreatorFunc> registry;
        return registry;
    }
};