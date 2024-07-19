#pragma once
#include "Cup.h"

#include "../Scene/SceneHierarchy.h"

#define ENGINE Cup::CupEngine::Instance()

namespace Cup {

	class EditorLayer : public Layer
	{
	public:
		explicit EditorLayer(CupEngine* instance);

		void OnAttach() override;

		void OnDetach() override;

		void OnUpdate(float deltatime) override;

		void OnImGuiRender() override;

		void RenderViewport();

	private:

		CupEntity m_cameraEntity;
		CupEntity m_meshEntity;
		SceneHierarchy m_sceneHierarchy;

		bool m_isViewportFocus = false;
		bool m_isViewportHovered = false;

		std::string example;
		std::shared_ptr<Scene> m_mainScene;
		CupEngine* m_instance;
		std::shared_ptr<Cup::Camera> m_camera;
		Mesh<float> m_cube;
	};

}