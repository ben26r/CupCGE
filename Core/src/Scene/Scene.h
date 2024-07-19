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
		inline CupEntity& CreateEntity(const std::string& tag = "entity") { CupEntity entity(&m_registry); entity.AddComponent<TagComponent>(tag); return entity; }

		void Update(float deltatime);
		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);

	private:
		olc::Sprite sprites;
		Registry m_registry;
	};

}
