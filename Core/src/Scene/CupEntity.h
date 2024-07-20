#pragma once
#include <vector>

#include "Core/Base.h"
#include "Register.h"
#include <string>

namespace Cup {

	class Scene;

	class CupEntity
	{
	public:

		CupEntity() = default;
		CupEntity(std::shared_ptr<Scene>& _scene, const std::string& tag = "EmptyTransform");

		template<typename T, typename ... Args>
		inline T& AddComponent(Args&&... args)
		{
			return m_scene->m_registry.AddComponent<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T>
		inline T& GetComponent()
		{
			return m_scene->m_registry.GetComponent<T>(m_entity);
		}

		template<typename T>
		inline bool HasComponent()
		{
			return m_scene->m_registry.HasComponent<T>(m_entity);
		}

		inline Entity GetIndex() { return m_entity; }

	protected:
		std::shared_ptr<Scene> m_scene = nullptr;
		Entity m_entity = -1;
	};

}