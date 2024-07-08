
#include "Cup.h"
#include "Core/EntryPoint.h"

#include <iostream>

#define ENGINE Cup::CupEngine::Instance()

class SandboxLayer : public Cup::Layer
{
public:
	void OnAttach() override 
	{
		if (!m_cube.LoadModel("assets/mountains.obj"))
		{

		}

		for (auto& tri : m_cube.triangles)
		{
			tri.color = Cup::Vector4(155, 155, 155, 255);
		}

		m_camera = std::make_shared<Cup::Camera>(ENGINE.GetAspectRatio());
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

		Cup::Matrix4x4<float> modelMatrix = Cup::Matrix4x4<float>::Translation(0.0f, -5.0f, -50.0f);
		Cup::Renderer::Submit<float>(m_camera, modelMatrix, m_cube);
	}

private:
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