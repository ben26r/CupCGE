#include "Core/CupEngine.h"
#include <algorithm>
#include <list>

namespace Cup {

	CupEngine* CupEngine::s_instance = nullptr;

	CupEngine::CupEngine()
	{
		CUP_ASSERT(s_instance == nullptr, "There can only be on instance of CupEngine!");
		s_instance = this;
	}

	bool CupEngine::OnUserCreate()
	{
		//m_cube.triangles = {
		//	// SOUTH
		//	{ Vector3(0.0f, 0.0f, 0.0f),    Vector3(0.0f, 1.0f, 0.0f),    Vector3(1.0f, 1.0f, 0.0f) },
		//	{ Vector3(0.0f, 0.0f, 0.0f),    Vector3(1.0f, 1.0f, 0.0f),    Vector3(1.0f, 0.0f, 0.0f) },
		//	// EAST
		//	{ Vector3(1.0f, 0.0f, 0.0f),    Vector3(1.0f, 1.0f, 0.0f),    Vector3(1.0f, 1.0f, 1.0f) },
		//	{ Vector3(1.0f, 0.0f, 0.0f),    Vector3(1.0f, 1.0f, 1.0f),    Vector3(1.0f, 0.0f, 1.0f) },
		//	// NORTH
		//	{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(1.0f, 1.0f, 1.0f),    Vector3(0.0f, 1.0f, 1.0f) },
		//	{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(0.0f, 1.0f, 1.0f),    Vector3(0.0f, 0.0f, 1.0f) },
		//	// WEST
		//	{ Vector3(0.0f, 0.0f, 1.0f),    Vector3(0.0f, 1.0f, 1.0f),    Vector3(0.0f, 1.0f, 0.0f) },
		//	{ Vector3(0.0f, 0.0f, 1.0f),    Vector3(0.0f, 1.0f, 0.0f),    Vector3(0.0f, 0.0f, 0.0f) },
		//	// TOP
		//	{ Vector3(0.0f, 1.0f, 0.0f),    Vector3(0.0f, 1.0f, 1.0f),    Vector3(1.0f, 1.0f, 1.0f) },
		//	{ Vector3(0.0f, 1.0f, 0.0f),    Vector3(1.0f, 1.0f, 1.0f),    Vector3(1.0f, 1.0f, 0.0f) },
		//	// BOTTOM
		//	{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(0.0f, 0.0f, 1.0f),    Vector3(0.0f, 0.0f, 0.0f) },
		//	{ Vector3(1.0f, 0.0f, 1.0f),    Vector3(0.0f, 0.0f, 0.0f),    Vector3(1.0f, 0.0f, 0.0f) },

		//};
		CUP_ASSERT(Renderer::Init(ScreenWidth(), ScreenHeight()), "Failed to initialize Renderer!");

		return true;
	}

	bool CupEngine::OnUserUpdate(float fElapsedTime)
	{
		
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));

		//m_theta += 1.0f * fElapsedTime;

		for (auto& layer : m_layerstack)
			layer->OnUpdate(fElapsedTime);

		// temporary renderer
		std::sort(Renderer::m_rendererData.sumTriangles.begin(), Renderer::m_rendererData.sumTriangles.end(), [](const Triangle<float>& triangle, const Triangle<float>& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});

		// temporary renderer
		for (const Triangle<float>& triangle : Renderer::m_rendererData.sumTriangles)
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
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					case 0:	nTrisToAdd = ClipAgainstPlane<float>({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = ClipAgainstPlane<float>({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = ClipAgainstPlane<float>({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = ClipAgainstPlane<float>({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}


			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto& t : listTriangles)
			{
				DrawCupTriangle(t, t.color);
			}
		}

		// temporary renderer
		Renderer::m_rendererData.sumTriangles.clear();

		return true;
	}

}