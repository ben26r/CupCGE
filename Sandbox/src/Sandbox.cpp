
#include "Cup.h"
#include "Core/EntryPoint.h"


#include <iostream>

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
	}
	void OnDetach() override {}
	void OnUpdate(float deltatime) override
	{
		std::cout << "updating " << m_debugName << "\n";
		Cup::CupEngine::Instance().Submit(m_cube);
	}

private:
	Cup::Mesh m_cube;
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