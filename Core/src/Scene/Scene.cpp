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
		{
			auto& ecs = m_registry.GetAllComponents<CameraComponent>();
			for (auto& ec : ecs)
			{
				if (ec.component.mainCamera)
				{
					mainCamera = ec.component.camera;
					break;
				}
			}
		}

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