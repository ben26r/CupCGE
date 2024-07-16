#pragma once

#include "Cup.h"

namespace Cup {

	class SceneHierarchy
	{
	public:
		explicit SceneHierarchy(const std::shared_ptr<Scene>& scene);
		~SceneHierarchy() = default;

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity, const std::string& name);
	private:
		std::shared_ptr<Scene> m_context;
		int m_selectedEntity = -1;
	};

}