#include "Scene.h"

#include "Components.h"
#include "Graphics/Renderer.h"
#include "json.hpp"
#include <fstream>
#include "ComponentSerializer.h"

namespace Cup {

	Scene::Scene()
		: sprites("assets/Dirt.png")
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::Update(float deltatime)
	{
		std::shared_ptr<Camera> mainCamera;
		std::shared_ptr<Camera> light;
		m_registry.ForEachComponent<CameraComponent>([&](Entity entity, CameraComponent& component) 
			{
				if (component.mainCamera)
				{
					mainCamera = component.camera;
				}
			});

		{
			Renderer::Start(mainCamera);
			auto& ecs = m_registry.GetAllComponentsWith<MeshComponent>(family::type<TransformComponent>());
			for (auto& ec : ecs)
			{
				auto& transform = m_registry.GetComponent<TransformComponent>(ec.entity);
				Renderer::Submit(transform.GetTransform() , ec.component.mesh, &sprites);
			}
			Renderer::End();
		}

	}

	void Scene::Serialize(const std::string& filepath)
	{
		nlohmann::json scene;

		for (Entity e = 0; e < m_registry.GetEntityIndex(); e++)
		{
			nlohmann::json entity;
			entity["entity"] = e;
			m_registry.ForEachEntity(e, [&](auto& component) {
				ComponentSerializer::Serialize(entity, component);
				});
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

}