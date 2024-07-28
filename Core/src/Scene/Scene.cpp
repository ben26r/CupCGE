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

	CupEntity Scene::CreateEntity(const std::string& tag)
	{
		CupEntity entity(m_registry.CreateEntity(), this);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();
		return entity;
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
			newComponent.instance->Init(this, newEntity);
		}

		return newEntity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		if (m_registry.HasComponent<ScriptComponent>(entity))
		{
			auto& sc = m_registry.GetComponent<ScriptComponent>(entity);
			sc.Delete();
		}

		m_registry.ForEachEntity(entity, [&](auto& component) {
			using ComponentType = std::decay_t<decltype(component)>;
			m_registry.RemoveComponent<ComponentType>(entity);
			});
	}

	void Scene::Start()
	{
		m_mainCamera = -1;
		std::shared_ptr<Camera> camera = nullptr;
		m_registry.ForEachComponent<CameraComponent>([&](Entity entity, CameraComponent& component)
			{
				if (component.mainCamera)
				{
					camera = component.camera;
					m_mainCamera = entity;
				}
			});

		if (m_mainCamera != -1)
		{
			auto& transform = m_registry.GetComponent<TransformComponent>(m_mainCamera);
			Renderer::Start(camera, transform.position);
		}

		m_registry.ForEachComponent<BoxColliderComponent>([&](Entity entity, BoxColliderComponent& component)
			{
				auto& transform = m_registry.GetComponent<TransformComponent>(entity);
				if (component.autoScale)
					component.scale = transform.scale;
				Vector3f& scale = component.scale * 0.5f;
				component.origin = transform.position - scale;
				component.transformScale = transform.position + scale;
			});
	}

	void Scene::End()
	{

		Renderer::End();
	}

	void Scene::Update(float deltatime)
	{
		if (m_mainCamera != -1)
		{
			m_registry.ForEachComponent<MeshRendererComponent>([&](Entity entity, MeshRendererComponent& component) {
				CUP_ASSERT_FUNC(m_registry.HasComponent<TransformComponent>(entity), return, "Mesh Renderer Component must have a transform!!");
				if (m_registry.HasComponent<SpriteAnimatorComponent>(entity))
				{
					auto& animator = m_registry.GetComponent<SpriteAnimatorComponent>(entity);
					animator.animator.Tick(deltatime);
					component.texture = animator.animator.GetTexture();
				}

				const auto& transform = m_registry.GetComponent<TransformComponent>(entity);
				Renderer::Submit(component.mesh, transform.GetTransform(), component.texture, component.tiling, component.color, component.wired);
				});

			m_registry.ForEachComponent<SpriteRendererComponent>([&](Entity entity, SpriteRendererComponent& component) {
				CUP_ASSERT_FUNC(m_registry.HasComponent<TransformComponent>(entity), return, "Sprite Renderer Component must have a transform!!");
				if (m_registry.HasComponent<SpriteAnimatorComponent>(entity))
				{
					auto& animator = m_registry.GetComponent<SpriteAnimatorComponent>(entity);
					animator.animator.Tick(deltatime);
					component.texture = animator.animator.GetTexture();
				}

				const auto& transform = m_registry.GetComponent<TransformComponent>(entity);
				Renderer::DrawSprite(component.texture, transform.GetTransform(), component.tiling, component.color, component.wired);
				});

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

	void Scene::Clear()
	{
		m_registry.ForEachComponent<ScriptComponent>([&](Entity entity, ScriptComponent& component) {
			component.Delete();
			});

		m_registry = Registry();
	}

	CupEntity Scene::FindEntityWithTag(const std::string& tag)
	{
		auto& entities = m_registry.GetAllComponents<TagComponent>();
		for (auto& entity : entities)
		{
			if (entity.component.tag == tag)
				return  { entity.entity, this };
		}
		CUP_ASSERT(false, "No entity with tag was found!");
	}

}