#pragma once

#include "Cup.h"

namespace Cup {

    class GameManager : public ScriptableEntity
    {
    public:

        void Start() override
        {
            s_instance = this;
        }

        void Update(float deltatime) override
        {
        }

        static inline GameManager* Instance() { return s_instance; }

    public:
        uint32_t keys = 0;
    private:
        static GameManager* s_instance;

    };

    GameManager* GameManager::s_instance = nullptr;

    REGISTER_SCRIPT(GameManager);

}
