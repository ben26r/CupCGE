#pragma once

#include "Cup.h"

#include "EnemyController.h"

namespace Cup {

    class GunManager : public ScriptableEntity
    {
    private:
        CupEntity m_player;
    public:

        void Start() override
        {
            m_player = m_scene->FindEntityWithTag("Camera");
        }

        void Update(float deltatime) override
        {
            if (Input::GetKeyPressed(olc::Key::SPACE))
            {
                auto& transform = m_player.GetComponent<TransformComponent>();
                auto& camera = m_player.GetComponent<CameraComponent>().camera;

                Linecast linecast(transform.position, camera->m_lookDir);
                Renderer::DrawLine(transform.position - Vector3f::Up(), linecast.position - Vector3f::Up());

                CupEntity& entity = linecast.entity;
                if (entity.GetComponent<TagComponent>().tag == "Enemy")
                {
                    EnemyController& enemyController = *static_cast<EnemyController*>(entity.GetComponent<ScriptComponent>().instance);
                    enemyController.Damage(10);
                }
            }
        }



    };

    REGISTER_SCRIPT(GunManager);

}
