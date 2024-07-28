#pragma once

#include "Cup.h"

#include "GameManager.h"

namespace Cup {

    class Key : public ScriptableEntity
    {
    public:

        void Start() override
        {
            player = m_scene->FindEntityWithTag("Camera");
        }

        void Update(float deltatime) override
        {
            auto& transform = GetComponent<TransformComponent>();
            auto& cameraTrans = player.GetComponent<TransformComponent>();

            Vector3f& direction = cameraTrans.position - transform.position;
            float distance = direction.magnitude();

            if (distance < 3)
            {
                GameManager::Instance()->keys += 1;
                std::cout << GameManager::Instance()->keys << "\n";
                transform.position.x += 10;
            }
        }

    public:
        CupEntity player;
    };

    REGISTER_SCRIPT(Key);

}