#pragma once

#include "Core/Base.h"
#include "Camera.h"
#include "Math/CupMath.h"

namespace Cup {

	class Renderer
	{
	public:

		static bool Init(const float& width, const float& height);

		template <typename T>
		static void Submit(const std::shared_ptr<Camera>& camera, const Matrix4x4<T>& matrix, const Mesh<T>& mesh);
	// temporary
	public:
		struct RendererData
		{
			std::vector<Triangle<float>> sumTriangles;

			float screenWidth = 1000;
			float screenHeight = 1000;
		};

		static RendererData m_rendererData;
	};

    template <typename T>
    void Renderer::Submit(const std::shared_ptr<Camera>& camera, const Matrix4x4<T>& matrix, const Mesh<T>& mesh)
    {
        for (const auto& triangle : mesh.triangles)
        {
            Triangle<T> triProj = triangle;

            triProj *= matrix;

            Vector3<T> normal = (triProj[1] - triProj[0]).cross((triProj[2] - triProj[0])).normalize();

            if (normal.dot(triProj[0] - camera->GetPosition()) < 0.0f)
            {
                Vector3<T> lightDirection = Vector3<T>(0, 1.0f, -1.0f);
                lightDirection = lightDirection.normalize();
                float dp = std::max(0.1f, normal.dot(lightDirection));

                triProj *= camera->GetView();

                int nClippedTriangles = 0;
                Triangle<T> clipped[2];
                nClippedTriangles = ClipAgainstPlane<float>({ 0.0f, 0.0f, 2.0f }, { 0.0f, 0.0f, 1.0f }, triProj, clipped[0], clipped[1]);

                for (int n = 0; n < nClippedTriangles; n++)
                {
                    for (Vector3<T>& vertex : clipped[n].vertices) {
                        vertex = camera->GetProjection() * vertex;
                        vertex = vertex / vertex.w;
                        vertex.x += 1.0f; vertex.y += 1.0f;
                        vertex.x *= 0.5f * m_rendererData.screenWidth; vertex.y *= 0.5f * m_rendererData.screenHeight;
                    }

                    clipped[n].color = triangle.color * dp;
                    m_rendererData.sumTriangles.push_back(clipped[n]);
                }
            }
        }
    }
}