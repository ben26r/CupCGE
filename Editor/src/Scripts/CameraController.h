#pragma once

#include "Cup.h"

namespace Cup {

    class CameraController : public ScriptableEntity
    {
    public:

        void Update(float deltatime) override
        {
            auto& m_camera = GetComponent<CameraComponent>().camera;

            if (CupEngine::Instance().GetKey(olc::Key::LEFT).bHeld)  m_camera->m_position.x -= 10.0f * deltatime;
            if (CupEngine::Instance().GetKey(olc::Key::RIGHT).bHeld) m_camera->m_position.x += 10.0f * deltatime;
            if (CupEngine::Instance().GetKey(olc::Key::UP).bHeld)    m_camera->m_position.y += 10.0f * deltatime;
            if (CupEngine::Instance().GetKey(olc::Key::DOWN).bHeld)  m_camera->m_position.y -= 10.0f * deltatime;

            Cup::Vector3<float> foward = m_camera->m_lookDir * 10 * deltatime;

            auto newPosition = m_camera->m_position;
            int fowardInput = 0;

            if (CupEngine::Instance().GetKey(olc::Key::S).bHeld)
                fowardInput -= 1;

            if (CupEngine::Instance().GetKey(olc::Key::W).bHeld)
                fowardInput += 1;
            m_camera->m_position += foward * fowardInput;

            if (CupEngine::Instance().GetKey(olc::Key::A).bHeld)
                m_camera->m_yaw -= 180.0f * deltatime;

            if (CupEngine::Instance().GetKey(olc::Key::D).bHeld)
                m_camera->m_yaw += 180.0f * deltatime;

            m_camera->m_yaw = m_camera->m_yaw < 0 ? 360.0f : m_camera->m_yaw > 360 ? 0 : m_camera->m_yaw;

            float scaleA = 1.5f * 0.5f;
            Vector3f origin = m_camera->m_position - scaleA;
            Vector3f scale = m_camera->m_position + scaleA;

            auto& colliders = m_scene->GetRegistry().GetAllComponents<BoxColliderComponent>();
            for (auto& entity : colliders)
            {
                if (CheckCollision(entity.component.origin, entity.component.transformScale, origin, scale))
                {
                    auto& strafe = (Matrix4x4f::Rotation(Vector3f::Up(), std::round(m_camera->m_yaw / 90) * 90) * Vector3f::Far());

                    m_camera->m_position = newPosition;
                    m_camera->m_position += strafe * 7 * deltatime * fowardInput;

                    for (auto& entity : colliders)
                    {
                        if (CheckCollision(entity.component.origin, entity.component.transformScale, m_camera->m_position - scaleA, m_camera->m_position + scaleA))
                        {
                            m_camera->m_position = newPosition;
                            break;
                        }
                    }
                    break;
                }
            }

            m_camera->RecalulateView();
        }

    };

    REGISTER_SCRIPT(CameraController);

}
