#pragma once

#include "Cup.h"

namespace Cup {

    class CameraController : public ScriptableEntity
    {
    public:

        void Update(float deltatime) override
        {
            auto& m_camera = GetComponent<CameraComponent>().camera;
            auto& transform = GetComponent<TransformComponent>();

            if (Input::GetKeyDown(olc::Key::LEFT))  transform.position.x -= 10.0f * deltatime;
            if (Input::GetKeyDown(olc::Key::RIGHT)) transform.position.x += 10.0f * deltatime;
            if (Input::GetKeyDown(olc::Key::UP))    transform.position.y += 10.0f * deltatime;
            if (Input::GetKeyDown(olc::Key::DOWN))  transform.position.y -= 10.0f * deltatime;

            Cup::Vector3<float> foward = m_camera->m_lookDir * 10 * deltatime;

            auto newPosition = transform.position;
            int fowardInput = 0;

            if (Input::GetKeyDown(olc::Key::S))
                fowardInput -= 1;

            if (Input::GetKeyDown(olc::Key::W))
                fowardInput += 1;

            transform.position += foward * fowardInput;

            if (Input::GetKeyDown(olc::Key::A))
                m_camera->m_yaw -= 180.0f * deltatime;

            if (Input::GetKeyDown(olc::Key::D))
                m_camera->m_yaw += 180.0f * deltatime;

            m_camera->m_yaw = m_camera->m_yaw < 0 ? 360.0f : m_camera->m_yaw > 360 ? 0 : m_camera->m_yaw;

            float scaleA = 0.25f * 0.5f;
            Vector3f origin = transform.position - scaleA;
            Vector3f scale = transform.position + scaleA;

            auto& colliders = m_scene->GetRegistry().GetAllComponents<BoxColliderComponent>();
            for (auto& entity : colliders)
            {
                if (CheckCollision(entity.component.origin, entity.component.transformScale, origin, scale))
                {
                    auto& strafe = (Matrix4x4f::Rotation(Vector3f::Up(), std::round(m_camera->m_yaw / 90) * 90) * Vector3f::Far());

                    transform.position = newPosition;
                    transform.position += strafe * 7 * deltatime * fowardInput;

                    for (auto& entity : colliders)
                    {
                        if (CheckCollision(entity.component.origin, entity.component.transformScale, transform.position - scaleA, transform.position + scaleA))
                        {
                            transform.position = newPosition;
                            break;
                        }
                    }
                    break;
                }
            }
        }

    };

    REGISTER_SCRIPT(CameraController);

}
