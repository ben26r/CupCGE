#include "Scene.h"
#include <fstream>

#include "Core/CupEngine.h"
#include "Graphics/Renderer.h"
#include "Components.h"
#include "ComponentSerializer.h"
#include "Script/ScriptRegistry.h"
#include "json.hpp"

namespace Cup {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = m_registry.CreateEntity();
		m_registry.ForEachEntity(entity, [&](auto& component) {
			using ComponentType = std::decay_t<decltype(component)>;
			if (family::type<ComponentType>() != family::type<ScriptComponent>())
			{

				m_registry.AddComponent<ComponentType>(newEntity) = component;
			}
			});

		if (m_registry.HasComponent<ScriptComponent>(entity))
		{
			ScriptComponent& component = m_registry.GetComponent<ScriptComponent>(entity);
			ScriptComponent& newComponent = m_registry.AddComponent<ScriptComponent>(newEntity);

			newComponent.instance = ScriptRegistry::Instance().CreateScript(component.scriptType);;
			newComponent.scriptType = component.scriptType;
			newComponent.instance->Init(CupEngine::ActiveScene(), newEntity);
		}

		return newEntity;
	}

	void Scene::Start()
	{
		m_registry.ForEachComponent<CameraComponent>([&](Entity entity, CameraComponent& component)
			{
				if (component.mainCamera)
				{
					m_mainCamera = component.camera;
				}
			});

		if (m_mainCamera)
		{
			Renderer::Start(m_mainCamera);
		}

		m_registry.ForEachComponent<BoxColliderComponent>([&](Entity entity, BoxColliderComponent& component)
			{
				TransformComponent& transform = m_registry.GetComponent<TransformComponent>(entity);
				if (component.autoScale)
					component.scale = transform.scale;
				component.origin = transform.position;
				component.transformScale = transform.position + component.scale;
			});
	}

	void Scene::End()
	{

		Renderer::End();
	}

	void Scene::Update(float deltatime)
	{
		if (m_mainCamera)
		{
			auto& ecs = m_registry.GetAllComponentsWith<MeshRendererComponent>(family::type<TransformComponent>());
			for (const auto& ec : ecs)
			{
				const auto& transform = m_registry.GetComponent<TransformComponent>(ec.entity);
				Renderer::Submit(transform.GetTransform(), ec.component);
			}

			m_registry.ForEachComponent<ScriptComponent>([&](Entity entity, ScriptComponent& component)
				{
					if (!component.started)
					{
						component.instance->Start();
						component.started = true;
					}

					component.instance->Update(deltatime);
				});
		}

	}

	void Scene::Serialize(const std::string& filepath)
	{
		nlohmann::json scene;

		for (Entity e = 0; e < m_registry.GetEntityIndex(); e++)
		{
			nlohmann::json entity;
			entity["entity"] = e;

			bool hasComponent = false;
			m_registry.ForEachEntity(e, [&](auto& component) {
				ComponentSerializer::Serialize(entity, component);
				hasComponent = true;
				});
			if (hasComponent)
				scene["entities"].push_back(entity);
		}

		std::ofstream file(filepath);
		file << scene.dump(4);
	}

	void Scene::Deserialize(const std::string& filepath)
	{
		std::ifstream file(filepath);
		nlohmann::json scene;
		file >> scene;

		for (const auto& entityJson : scene["entities"]) 
		{
			Entity entity = m_registry.CreateEntity();
			m_registry.ForEachType([&](auto& component) {
				ComponentSerializer::Deserialize(entityJson, component, m_registry, entity);
				});
		}
	}

	Entity Scene::FindEntityWithTag(const std::string& tag)
	{
		auto& entities = m_registry.GetAllComponents<TagComponent>();
		for (auto& entity : entities)
		{
			if (entity.component.tag == tag)
				return  entity.entity;
		}
		CUP_ASSERT(false, "No entity with tag was found!");
		return -1;
	}

}