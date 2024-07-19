
#include "Cup.h"
#include "Core/EntryPoint.h"

#include <iostream>

#define ENGINE Cup::CupEngine::Instance()
#define SCENE Cup::CupEngine::MainScene()

class SandboxLayer : public Cup::Layer
{
public:
	void OnAttach() override 
	{
		SCENE->Deserialize("assets/scene.json");

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