#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include "Scene/ScriptableEntity.h"

namespace Cup {

    using ScriptCreator = std::function<ScriptableEntity* ()>;

    class ScriptRegistry
    {
    public:
        static ScriptRegistry& Instance()
        {
            static ScriptRegistry instance;
            return instance;
        }

        inline const std::unordered_map<std::string, ScriptCreator>& GetRegistry() const { return m_registry; }

        template<typename T>
        inline void RegisterScript(const std::string& name)
        {
            m_registry[name] = []() { return new T(); };
        }

        ScriptableEntity* CreateScript(const std::string& name);

    private:
        ScriptRegistry() = default;
        std::unordered_map<std::string, ScriptCreator> m_registry;
    };

#define REGISTER_SCRIPT(T) \
    static bool T##_registered = []() { \
        ScriptRegistry::Instance().RegisterScript<T>(#T); \
        return true; \
    }();

}