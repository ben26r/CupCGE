#include "Renderer.h"

namespace Cup {

    Renderer::RendererData Renderer::m_rendererData = Renderer::RendererData();

    bool Renderer::Init(olc::PixelGameEngine* appPtr) // - temporary solution
    {
		SCOPE_FUNC();
		m_rendererData.appPtr = appPtr;
        return true;
    }

	void Renderer::Submit(const std::shared_ptr<Camera>& camera, const Matrix4x4f& matrix, const Meshf& mesh)
	{
		SCOPE_FUNC();

		for (auto triangle : mesh.triangles)
		{
			triangle *= matrix;

			Vector3f normal((triangle[1] - triangle[0]).cross((triangle[2] - triangle[0])));
			normal.normalize();

			if (normal.dot(triangle[0] - camera->GetPosition()) < 0.0f)
			{
				Vector3f lightDirection = Vector3f(0, 1.0f, -1.0f);
				lightDirection.normalize();
				float dp = std::max(0.5f, normal.dot(lightDirection));

				triangle *= camera->GetView();

				int nClippedTriangles = 0;
				Trianglef clipped[2];

				Vector3f planeN(0.0f, 0.0f, 1.0f);
				planeN.normalize();
				float planeD = -planeN.dot({ 0.0f, 0.0f, 2.0f });
				nClippedTriangles = ClipAgainstPlane<float>(-2.0f, { 0.0f, 0.0f, 1.0f }, triangle, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++)
				{
					for (Vector3f& vertex : clipped[n].vertices) {
						vertex = camera->GetProjection() * vertex;
						vertex = vertex / vertex.w;
						vertex.x += 1.0f; vertex.y += 1.0f;
						vertex.x *= 0.5f * (float)m_rendererData.appPtr->ScreenWidth(); vertex.y *= 0.5f * (float)m_rendererData.appPtr->ScreenHeight();
					}

					clipped[n].color = clipped[n].color * dp;
					m_rendererData.sumTriangles.push_back(clipped[n]);
				}
			}
		}
	}

	void Renderer::Start()
	{
		m_rendererData.appPtr->FillRect(0, 0, m_rendererData.appPtr->ScreenWidth(), m_rendererData.appPtr->ScreenHeight(), olc::Pixel(0, 0, 0));
	}

	void Renderer::End()
	{
		SCOPE_FUNC();
		Sort();

		Vector3f planeN(0.0f, -1.0f, 0.0f);
		planeN.normalize();
		float planeD = -planeN.dot({ 0.0f,  (float)m_rendererData.appPtr->ScreenHeight() - 1, 0.0f });

		Vector3f planeN2(-1.0f, 0.0f, 0.0f);
		planeN.normalize();
		float planeD2 = -planeN2.dot({ (float)m_rendererData.appPtr->ScreenWidth() - 1, 0.0f, 0.0f });

		for (const Triangle<float>& triangle : m_rendererData.sumTriangles)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			Triangle<float> clipped[2];
			std::list<Triangle<float>> listTriangles;

			// Add initial triangle
			listTriangles.push_back(triangle);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					Triangle<float> test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane.
					switch (p)
					{
					case 0:	nTrisToAdd = ClipAgainstPlane<float>(0.0f, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = ClipAgainstPlane<float>(planeD, planeN, test, clipped[0], clipped[1]); break;

					case 2:	nTrisToAdd = ClipAgainstPlane<float>(0.0f, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = ClipAgainstPlane<float>(planeD2, planeN2, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}

			for (auto& t : listTriangles)
			{
				DrawCupTriangle(t, t.color);
			}
		}

		m_rendererData.sumTriangles.clear();
	}

	void Renderer::DrawCupTriangle(const Trianglef& triangle, const Vector4& color)
	{
		SCOPE_FUNC();
		m_rendererData.appPtr->FillTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(color.x, color.y, color.z, 255.0f));
		m_rendererData.appPtr->DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(0.0f, 0.0f, 0.0f, 255.0f));
	}

	void Renderer::Sort()
	{
		SCOPE_FUNC();
		std::sort(m_rendererData.sumTriangles.begin(), m_rendererData.sumTriangles.end(), [](const Triangle<float>& triangle, const Triangle<float>& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});
	}

}