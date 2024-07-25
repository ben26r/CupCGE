#pragma once

#include "Core/CupEngine.h"

#include "CupMath.h"
#include <algorithm>

namespace Cup {

	struct Raycast
	{
		float distance = 1000;
		bool hit = false;

		Raycast(const Vector3f& _origin, const Vector3f& _direction)
		{
			glm::vec3 origin = { _origin.x, _origin.y, _origin.z };
			glm::vec3 direction = { _direction.x, _direction.y, _direction.z };
			
			const auto& scene = CupEngine::ActiveScene();
			std::vector<float> hits;
			scene->GetRegistry().ForEachComponentWith<MeshRendererComponent>(family::type<TransformComponent>(), [&](Entity entity, MeshRendererComponent& mesh) {
				auto& transform = scene->GetRegistry().GetComponent<TransformComponent>(entity).GetTransform();
				for (auto triangle : mesh.mesh.triangles)
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
						}
					}
				}
				});
		}
	};

	struct Linecast
	{
		Vector3f position;
		float distance = 1000;
		bool hit = false;

		Linecast(const Vector3f& _origin, const Vector3f& _direction)
		{
			glm::vec3 origin = { _origin.x, _origin.y, _origin.z };
			glm::vec3 direction = { _direction.x, _direction.y, _direction.z };

			const auto& scene = CupEngine::ActiveScene();
			std::vector<float> hits;
			scene->GetRegistry().ForEachComponentWith<MeshRendererComponent>(family::type<TransformComponent>(), [&](Entity entity, MeshRendererComponent& mesh) {
				auto& transform = scene->GetRegistry().GetComponent<TransformComponent>(entity).GetTransform();
				for (auto triangle : mesh.mesh.triangles)
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
							position = _origin + (_direction * d);
						}
					}
				}
				});
		}
	};
}