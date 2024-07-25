#pragma once

#include "Cup.h"

namespace Cup {

    class EnemyController : public ScriptableEntity
    {
    private:
        Entity camera;
    public:

        void Start() override
        {
            camera = m_scene->FindEntityWithTag("Camera");
        }

        void Update(float deltatime) override
        {
            auto& transform = GetComponent<TransformComponent>();
            auto& cameraTransform = m_scene->GetRegistry().GetComponent<CameraComponent>(camera);

            Vector3f& direction = cameraTransform.camera->m_position - transform.position;
            float distance = direction.magnitude();
            Vector3f& foward = (cameraTransform.camera->m_position - transform.position).normalize() * 5 * deltatime;

            if (distance > 5)
                transform.position += foward;
            else if (distance < 4)
                transform.position -= foward;

            //float scaleA = 1.5f * 0.5f;
            //Vector3f origin = m_camera->m_position - scaleA;
            //Vector3f scale = m_camera->m_position + scaleA;

            //auto& colliders = m_scene->GetRegistry().GetAllComponents<BoxColliderComponent>();
            //for (auto& entity : colliders)
            //{
            //    if (CheckCollision(entity.component.origin, entity.component.transformScale, origin, scale))
            //    {
            //        auto& strafe = (Matrix4x4f::Rotation(Vector3f::Up(), std::round(m_camera->m_yaw / 90) * 90) * Vector3f::Far());

            //        m_camera->m_position = newPosition;
            //        m_camera->m_position += strafe * 7 * deltatime * fowardInput;

            //        for (auto& entity : colliders)
            //        {
            //            if (CheckCollision(entity.component.origin, entity.component.transformScale, m_camera->m_position - scaleA, m_camera->m_position + scaleA))
            //            {
            //                m_camera->m_position = newPosition;
            //                break;
            //            }
            //        }
            //        break;
            //    }
            //}
        }

    };

    REGISTER_SCRIPT(EnemyController);
}