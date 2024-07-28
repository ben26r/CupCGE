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

		CupEntity CreateEntity(const std::string& tag = "entity");
		inline Registry& GetRegistry() { return m_registry; }
		Entity DuplicateEntity(Entity entity);
		void DeleteEntity(Entity entity);

		void Start();
		void End();

		void Update(float deltatime);
		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);
		void Clear();

		CupEntity FindEntityWithTag(const std::string& tag);

	private:
		Registry m_registry;
		std::vector<Vector3f> m_collisionPositions;
		int m_mainCamera = -1;
	};

}
