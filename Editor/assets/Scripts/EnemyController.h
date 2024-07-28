#pragma once

#include "Cup.h"

namespace Cup {

    class EnemyController : public ScriptableEntity
    {
    private:
        CupEntity m_player;
        float timer = 0;
        float deathDelay = 0;

        float health = 10;

        CupEntity explosionEffect;
        bool isDead = false;
    public:

        void Damage(float power)
        {
            health -= power;
            std::cout << health << "\n";
            if (health <= 0)
            {
                RemoveComponent<MeshColliderComponent>();
                RemoveComponent<SpriteRendererComponent>();

                explosionEffect = m_scene->CreateEntity();
                explosionEffect.AddComponent<SpriteRendererComponent>();

                auto& transform = GetComponent<TransformComponent>();
                auto& explosionTrans = explosionEffect.GetComponent<TransformComponent>();
                Vector3f newPos = transform.position;
                newPos.x += transform.scale.x * 0.5f;
                newPos.z += transform.scale.z * 0.5f - 0.5f;
                explosionTrans.position = newPos;

                deathDelay = 1;
                isDead = true;
            }
        }

        void Start() override
        {
            m_player = m_scene->FindEntityWithTag("Camera");
        }

        void Update(float deltatime) override
        {
            auto& playerTrans = m_player.GetComponent<TransformComponent>();

            if (isDead)
            {
                auto& explosionTrans = explosionEffect.GetComponent<TransformComponent>();
                explosionTrans.scale += 1.5f * deltatime;

                Vector3f& direction = playerTrans.position - explosionTrans.position;
                float angleRadians = atan2(direction.z, direction.x);
                float angleDegrees = angleRadians * (180.0 / 3.14159f);
                angleDegrees += 90;

                explosionTrans.rotation.y = angleDegrees;

                deathDelay -= deltatime;
                if (deathDelay < 0)
                {
                    m_scene->DeleteEntity(explosionEffect.GetIndex());
                    m_scene->DeleteEntity(m_entity);
                }

                return;
            }
            auto& transform = GetComponent<TransformComponent>();
            Vector3f& direction = playerTrans.position - transform.position;

            float angleRadians = atan2(direction.z, direction.x);
            float angleDegrees = angleRadians * (180.0 / 3.14159f);
            transform.rotation.y = angleDegrees + 90;
            angleDegrees -= 90;

            float distance = direction.magnitude();
            Vector3f forward = direction;
            forward.normalize();
            forward.y = 0;
            float speed = 8;

            Vector3f ogPosition = transform.position;

            int dir = 0;

            if (distance > 12)
                dir = 1;
            else if (distance < 6)
                timer = 0.4f;

            if (timer > 0)
            {
                dir = -1;
                speed = 18;
                timer -= deltatime;
            }

            transform.position += forward * speed * deltatime * dir;

            auto& boxCollider = GetComponent<BoxColliderComponent>();
            Vector3f& s = boxCollider.scale * 0.5f;
            Vector3f& origin = transform.position - s;
            Vector3f& scale = transform.position + s;

            auto& colliders = m_scene->GetRegistry().GetAllComponents<BoxColliderComponent>();
            for (auto& entity : colliders)
            {
                if (entity.entity != m_entity)
                {
                    if (CheckCollision(entity.component.origin, entity.component.transformScale, origin, scale))
                    {
                        transform.position = ogPosition;

                        auto& strafe = (Matrix4x4f::Rotation(Vector3f::Up(), (std::round(angleDegrees / 90) * 90)) * Vector3f::Far());
                        Renderer::DrawLine(transform.position + strafe, transform.position + strafe * 2);
                        Renderer::DrawLine(transform.position + forward, transform.position + forward * 20, olc::RED);
                        transform.position += strafe * speed * deltatime * dir;

                        for (auto& entity : colliders)
                        {
                            if (entity.entity != m_entity)
                            {
                                if (CheckCollision(entity.component.origin, entity.component.transformScale, transform.position - s, transform.position + s))
                                {
                                    transform.position = ogPosition;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }

    };

    REGISTER_SCRIPT(EnemyController);
}