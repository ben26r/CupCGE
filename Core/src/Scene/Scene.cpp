#include "Scene.h"

#include "Components.h"
#include "Graphics/Renderer.h"

namespace Cup {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::Update(float deltatime)
	{
		std::shared_ptr<Camera> mainCamera;
		m_registry.ForEachComponent<CameraComponent>([&](Entity entity, CameraComponent& component) 
			{
				if (component.mainCamera)
				{
					mainCamera = component.camera;
				}
			});

		{
			auto& ecs = m_registry.GetAllComponentsWith<MeshComponent>(family::type<TransformComponent>());
			for (auto& ec : ecs)
			{
				auto& transform = m_registry.GetComponent<TransformComponent>(ec.entity);
				Renderer::Submit(mainCamera, transform.GetTransform() , ec.component.mesh);
			}
		}

	}

}