#pragma once

#include "Cup.h"

#include <vector>

namespace Cup {

	class SceneHierarchy
	{
	public:
		explicit SceneHierarchy(const std::shared_ptr<Scene>& scene);
		~SceneHierarchy() = default;

		void OnImGuiRender();
		const std::vector<int>& GetSelected() const { return m_selectedEntity; }
	private:
		void DrawComponents();
		void DrawEntityNode(Entity entity, const std::string& name);
	private:
		std::shared_ptr<Scene> m_context;
		std::vector<int> m_selectedEntity;
	};

}