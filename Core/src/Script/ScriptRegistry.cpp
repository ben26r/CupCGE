#include "ScriptRegistry.h"

namespace Cup {

    ScriptableEntity* ScriptRegistry::CreateScript(const std::string& name)
    {
        auto it = m_registry.find(name);
        if (it != m_registry.end())
        {
            return it->second();
        }
        return nullptr;
    }

}