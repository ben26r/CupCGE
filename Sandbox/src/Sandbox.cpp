
#include "Cup.h"
#include "Core/EntryPoint.h"

#include <iostream>

#define ENGINE Cup::CupEngine::Instance()
#define SCENE Cup::CupEngine::InsScene()

class SandboxLayer : public Cup::Layer
{
public:
	void OnAttach() override 
	{
		m_meshEntity = SCENE.CreateEntity();
		m_cameraEntity = SCENE.CreateEntity();

		m_cube.CreateCube();

		for (auto& tri : m_cube.triangles)
		{
			tri.color = Cup::Vector4(155, 155, 155, 255);
		}

		m_meshEntity.AddComponent<Cup::MeshComponent>(m_cube);
		m_meshEntity.AddComponent<Cup::TransformComponent>(Cup::Vector3f(0.0f, 0.0f, 10.0f), Cup::Vector3f(), Cup::Vector3f(1.0f, 1.0f, 1.0f));

		m_camera = std::make_shared<Cup::Camera>(0.8f);
		m_cameraEntity.AddComponent<Cup::CameraComponent>(m_camera, true);
	}

	void OnDetach() override {}

	void OnUpdate(float deltatime) override
	{
		if (ENGINE.GetKey(olc::Key::LEFT).bHeld)  m_camera->m_position.x -= 10.0f * deltatime;
		if (ENGINE.GetKey(olc::Key::RIGHT).bHeld) m_camera->m_position.x += 10.0f * deltatime;
		if (ENGINE.GetKey(olc::Key::UP).bHeld)    m_camera->m_position.y += 10.0f * deltatime;
		if (ENGINE.GetKey(olc::Key::DOWN).bHeld)  m_camera->m_position.y -= 10.0f * deltatime;


		Cup::Vector3<float> foward = m_camera->m_lookDir * 10 * deltatime;

		if (ENGINE.GetKey(olc::Key::S).bHeld)
			m_camera->m_position -= foward;

		if (ENGINE.GetKey(olc::Key::W).bHeld)
			m_camera->m_position += foward;

		if (ENGINE.GetKey(olc::Key::A).bHeld)
			m_camera->m_yaw -= 2.0f * deltatime;

		if (ENGINE.GetKey(olc::Key::D).bHeld)
			m_camera->m_yaw += 2.0f * deltatime;

		m_camera->RecalulateView();
	}

	void OnImGuiRender() override
	{
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{

			ImGui::Begin("Viewport");

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

			ImGui::Image((void*)2, ImVec2(viewportPanelSize.x, viewportPanelSize.y), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
		}
	}

private:
	Cup::CupEntity m_cameraEntity;
	Cup::CupEntity m_meshEntity;
	std::shared_ptr<Cup::Camera> m_camera;
	Cup::Mesh<float> m_cube;
};

namespace Cup {

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