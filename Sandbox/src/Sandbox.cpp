
#include "Cup.h"
#include "Core/EntryPoint.h"

#include <iostream>

#define ENGINE Cup::CupEngine::Instance()
#define SCENE Cup::CupEngine::ActiveScene()

namespace Cup {

class SandboxLayer : public Cup::Layer
{
public:
	void OnAttach() override 
	{
		SCENE->Deserialize("assets/scene.json");
		Cup::Renderer::GetTextureStorage().Deserialize("assets/resourcepack.json");

        class CameraController : public ScriptableEntity
        {
        public:

            void Update(float deltatime) override
            {
                auto& m_camera = GetComponent<CameraComponent>().camera;

                if (ENGINE.GetKey(olc::Key::LEFT).bHeld)  m_camera->m_position.x -= 10.0f * deltatime;
                if (ENGINE.GetKey(olc::Key::RIGHT).bHeld) m_camera->m_position.x += 10.0f * deltatime;
                if (ENGINE.GetKey(olc::Key::UP).bHeld)    m_camera->m_position.y += 10.0f * deltatime;
                if (ENGINE.GetKey(olc::Key::DOWN).bHeld)  m_camera->m_position.y -= 10.0f * deltatime;

                Cup::Vector3<float> foward = m_camera->m_lookDir * 10 * deltatime;

                auto newPosition = m_camera->m_position;
                int fowardInput = 0;

                if (ENGINE.GetKey(olc::Key::S).bHeld)
                    fowardInput -= 1;

                if (ENGINE.GetKey(olc::Key::W).bHeld)
                    fowardInput += 1;
                m_camera->m_position += foward * fowardInput;

                if (ENGINE.GetKey(olc::Key::A).bHeld)
                    m_camera->m_yaw -= 180.0f * deltatime;

                if (ENGINE.GetKey(olc::Key::D).bHeld)
                    m_camera->m_yaw += 180.0f * deltatime;

                m_camera->m_yaw = m_camera->m_yaw < 0 ? 360.0f : m_camera->m_yaw > 360 ? 0 : m_camera->m_yaw;

                float scaleA = 1.5f * 0.5f;
                Vector3f origin = m_camera->m_position - scaleA;
                Vector3f scale = m_camera->m_position + scaleA;
                auto& strafe = (Matrix4x4f::Rotation(Vector3f::Up(), std::round(m_camera->m_yaw / 90) * 90) * Vector3f::Far());

                bool hit = false;
                bool strafeHit = false;
                m_scene->GetRegistry().ForEachComponentWith<BoxColliderComponent>(family::type<TransformComponent>(), [&](Entity entity, BoxColliderComponent& component) {
                    auto& transform = m_scene->GetRegistry().GetComponent<TransformComponent>(entity);
                    if (CheckCollision(component.origin, component.transformScale, origin, scale))
                    {
                        m_camera->m_position = newPosition;
                        m_camera->m_position += strafe * 7 * deltatime * fowardInput;
                        if (CheckCollision(component.origin, component.transformScale, m_camera->m_position - scaleA, m_camera->m_position + scaleA))
                        {
                            m_camera->m_position = newPosition;
                        }
                    }
                    });

                m_camera->RecalulateView();
            }

        };
        Cup::Entity entity = SCENE->FindEntityWithTag("Camera");
        SCENE->GetRegistry().AddComponent<Cup::ScriptComponent>(entity).Bind<CameraController>(SCENE, entity);
        m_camera = SCENE->GetRegistry().GetComponent<Cup::CameraComponent>(entity).camera;
	}

	void OnDetach() override {}

	void OnUpdate(float deltatime) override
	{

	}

private:
	Cup::CupEntity m_cameraEntity;
	Cup::CupEntity m_meshEntity;
	std::shared_ptr<Cup::Camera> m_camera;
	Cup::Mesh<float> m_cube;
};

	class CupSandbox : public Cup::CupEngine
	{
	public:
		CupSandbox() {
			PushLayer(new SandboxLayer());
		}
		~CupSandbox() {

		}
	};

	Cup::CupEngine* Cup::CreateApplication() {
		return new CupSandbox;
	}

}