#include "CupEngine.h"

#include "ImGui/ImGuiCommand.h"
#include "Input.h"

namespace Cup {

	CupEngine* CupEngine::s_instance = nullptr;

	CupEngine::CupEngine()
		: m_activeScene(std::make_shared<Scene>())
	{
		CUP_ASSERT(s_instance == nullptr, "There can only be on instance of CupEngine!");
		s_instance = this;
		Input::Init(this);

		bool r = Renderer::Init(this);
		CUP_ASSERT(r, "Failed to initialize Renderer!");
	}

	bool CupEngine::OnUserDestroy()
	{
		m_layerstack.~LayerStack();
		return true;
	}

	bool CupEngine::OnUserCreate()
	{
		float x = 0;

		ImGuiCommand::Init(this);

		return true;
	}

	bool CupEngine::OnUserUpdate(float fElapsedTime)
	{
		//m_theta += 1.0f * fElapsedTime;
		//Renderer::Start();

		m_activeScene->Start();
		m_activeScene->Update(fElapsedTime);
		for (auto& layer : m_layerstack)
			layer->OnUpdate(fElapsedTime);
		m_activeScene->End();

		return true;
	}

	bool CupEngine::OnUserImGuiRender() {
		ImGuiCommand::Begin();
		for (auto& layer : m_layerstack)
			layer->OnImGuiRender();
		ImGuiCommand::End();
		return true;
	}

}