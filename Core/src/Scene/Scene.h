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
		Entity DuplicateEntity(Entity entity);

		void Start();
		void End();

		void Update(float deltatime);
		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);

		Entity FindEntityWithTag(const std::string& tag);

	private:
		Registry m_registry;
		std::vector<Vector3f> m_collisionPositions;
		std::shared_ptr<Camera> m_mainCamera;
	};

}
