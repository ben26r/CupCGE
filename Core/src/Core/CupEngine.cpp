#include "CupEngine.h"

namespace Cup {

	CupEngine* CupEngine::s_instance = nullptr;

	CupEngine::CupEngine()
	{
		CUP_ASSERT(s_instance == nullptr, "There can only be on instance of CupEngine!");
		s_instance = this;
	}

	bool CupEngine::OnUserCreate()
	{
		SetDrawTarget(nullptr);
		ImGuiCommand::Init(this);
		//ImGuiCommand::Init();:
		bool r = Renderer::Init(this);
		CUP_ASSERT(r, "Failed to initialize Renderer!");
		float x = 0;

		return true;
	}

	bool CupEngine::OnUserUpdate(float fElapsedTime)
	{

		//m_theta += 1.0f * fElapsedTime;
		Renderer::Start();

		for (auto& layer : m_layerstack)
			layer->OnUpdate(fElapsedTime);
		m_scene.Update(fElapsedTime);
		Renderer::End();

		return true;
	}

	bool CupEngine::OnUserRender()
	{
		ImGuiCommand::Begin(GetWindowSize());
		for (auto& layer : m_layerstack)
			layer->OnImGuiRender();
		ImGuiCommand::End();
		return false;
	}
}