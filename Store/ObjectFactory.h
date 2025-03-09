#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "IObject.h"

class ObjectFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<IObject>()>;

    static void Register(const std::string& name, CreatorFunc creator) {
        GetRegistry()[name] = std::move(creator);
    }

    static std::unique_ptr<IObject> Create(const std::string& name) {
        auto& registry = GetRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();  // Call the creation function
        }
        return nullptr;
    }

private:
    static std::unordered_map<std::string, CreatorFunc>& GetRegistry() {
        static std::unordered_map<std::string, CreatorFunc> registry;
        return registry;
    }
};