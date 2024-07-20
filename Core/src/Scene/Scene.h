#pragma once
#include "Core/Base.h"
#include "Register.h"
#include "CupEntity.h"
#include "Components.h"
#include <string>

namespace Cup {

	class Scene
	{
	private:
		friend class CupEntity;
		friend class SceneHierarchy;
	public:
		explicit Scene();
		~Scene();

		// CupEntity should use scene not registry
		//inline CupEntity& CreateEntity(const std::string& tag = "entity") { CupEntity entity(this); entity.AddComponent<TagComponent>(tag); return entity; }
		inline Registry& GetRegistry() { return m_registry; }

		void Update(float deltatime);
		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);

	private:
		Registry m_registry;
	};

}
