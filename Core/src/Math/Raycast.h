#pragma once

#include "Core/CupEngine.h"

#include "CupMath.h"
#include <algorithm>

namespace Cup {

	struct Raycast
	{
		CupEntity entity;
		float distance = 1000;
		bool hit = false;

		Raycast(const Vector3f& _origin, const Vector3f& _direction)
		{
			glm::vec3 origin = { _origin.x, _origin.y, _origin.z };
			glm::vec3 direction = { _direction.x, _direction.y, _direction.z };
			
			uint32_t e = 0;
			const auto& scene = CupEngine::ActiveScene();
			std::vector<float> hits;
			scene->GetRegistry().ForEachComponentWith<MeshColliderComponent>(family::type<TransformComponent>(), [&](Entity entity, MeshColliderComponent& component) {
				CUP_ASSERT_FUNC(scene->GetRegistry().HasComponent<MeshRendererComponent>(entity) || scene->GetRegistry().HasComponent<SpriteRendererComponent>(entity), return, "Mesh Collider must have a Mesh Renderer Component!!");
				const Meshf* mesh;
				if (scene->GetRegistry().HasComponent<SpriteRendererComponent>(entity))
					mesh = &Renderer::GetSpriteMesh();
				else
					mesh = &scene->GetRegistry().GetComponent<MeshRendererComponent>(entity).mesh;
				auto& transform = scene->GetRegistry().GetComponent<TransformComponent>(entity).GetTransform();
				for (auto triangle : mesh->triangles)
				{
					triangle *= transform;
					glm::vec3 v0(triangle[0].x, triangle[0].y, triangle[0].z);
					glm::vec3 v1(triangle[1].x, triangle[1].y, triangle[1].z);
					glm::vec3 v2(triangle[2].x, triangle[2].y, triangle[2].z);
					glm::vec2 baryPosition;
					float d = 0;
					if (glm::intersectRayTriangle(origin, direction, v0, v1, v2, baryPosition, d))
					{
						if (d > 0 && d < distance)
						{
							hit = true;
							distance = d; 
							e = entity;
						}
					}
				}
				});
			entity = { e, scene.get() };
		}
	};

	struct Linecast
	{
		CupEntity entity;
		Vector3f position;
		float distance = 1000;
		bool hit = false;

		Linecast(const Vector3f& _origin, const Vector3f& _direction)
		{
			glm::vec3 origin = { _origin.x, _origin.y, _origin.z };
			glm::vec3 direction = { _direction.x, _direction.y, _direction.z };

			uint32_t e = 0;
			const auto& scene = CupEngine::ActiveScene();
			std::vector<float> hits;
			scene->GetRegistry().ForEachComponentWith<MeshColliderComponent>(family::type<TransformComponent>(), [&](Entity entity, MeshColliderComponent& component) {
				CUP_ASSERT_FUNC(scene->GetRegistry().HasComponent<MeshRendererComponent>(entity) || scene->GetRegistry().HasComponent<SpriteRendererComponent>(entity), return, "Mesh Collider must have a Mesh Renderer Component!!");
				const Meshf* mesh;
				if (scene->GetRegistry().HasComponent<SpriteRendererComponent>(entity))
					mesh = &Renderer::GetSpriteMesh();
				else
					mesh = &scene->GetRegistry().GetComponent<MeshRendererComponent>(entity).mesh;
				auto& transform = scene->GetRegistry().GetComponent<TransformComponent>(entity);
				for (auto triangle : mesh->triangles)
				{
					triangle *= transform.GetTransform();
					glm::vec3 v0(triangle[0].x, triangle[0].y, triangle[0].z);
					glm::vec3 v1(triangle[1].x, triangle[1].y, triangle[1].z);
					glm::vec3 v2(triangle[2].x, triangle[2].y, triangle[2].z);
					glm::vec2 baryPosition;
					float d = 0;

					if (glm::intersectRayTriangle(origin, direction, v0, v1, v2, baryPosition, d))
					{
						if (d > 0 && d < distance)
						{
							e = entity;
							hit = true;
							distance = d;
							position = _origin + (_direction * d);
						}
					}
				}
				});
			entity = { e, scene.get() };
		}
	};
}