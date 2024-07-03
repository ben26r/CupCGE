#include "Core/CupEngine.h"
#include <algorithm>
#include <list>

namespace Cup {

	CupEngine* CupEngine::s_instance = nullptr;

	CupEngine::CupEngine()
	{
		if (s_instance != nullptr)
		{

		}
		s_instance = this;

		m_camera = std::make_shared<Camera>(GetAspectRatio());
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

		return true;
	}

	bool CupEngine::OnUserUpdate(float fElapsedTime)
	{
		
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));

		//m_theta += 1.0f * fElapsedTime;

		if (GetKey(olc::Key::LEFT).bHeld)  m_camera->m_position.x -= 10.0f * fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) m_camera->m_position.x += 10.0f * fElapsedTime;
		if (GetKey(olc::Key::UP).bHeld)    m_camera->m_position.y += 10.0f * fElapsedTime;
		if (GetKey(olc::Key::DOWN).bHeld)  m_camera->m_position.y -= 10.0f * fElapsedTime;


		Vector3 foward = m_camera->m_lookDir * 10 * fElapsedTime;

		if (GetKey(olc::Key::S).bHeld)
			m_camera->m_position -= foward;

		if (GetKey(olc::Key::W).bHeld)
			m_camera->m_position += foward;

		if (GetKey(olc::Key::A).bHeld)
			m_camera->m_yaw -= 2.0f * fElapsedTime;

		if (GetKey(olc::Key::D).bHeld)
			m_camera->m_yaw += 2.0f * fElapsedTime;

		m_camera->RecalulateView();

		for (auto& layer : m_layerstack)
			layer->OnUpdate(fElapsedTime);

		std::sort(m_sumtriangles.begin(), m_sumtriangles.end(), [](const Triangle& triangle, const Triangle& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});

		for (const Triangle& triangle : m_sumtriangles)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			Triangle clipped[2];
			std::list<Triangle> listTriangles;

			// Add initial triangle
			listTriangles.push_back(triangle);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					Triangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					case 0:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
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

		m_sumtriangles.clear();

		return true;
	}

	void CupEngine::Submit(const Mesh& mesh)
	{
		for (const auto& triangle : mesh.triangles)
		{
			Triangle triProj = triangle;

			Matrix4x4 modelMatrix = Matrix4x4::Translation(0.0f, -5.0f, -50.0f);// *Matrix4x4::Scale(10.0f, 1.0f, 10.0f);
			triProj *= modelMatrix;

			Vector3 normal = (triProj[1] - triProj[0]).cross((triProj[2] - triProj[0])).normalize();

			if (normal.dot(triProj[0] - m_camera->GetPosition()) < 0.0f)
			{
				Vector3 lightDirection = Vector3(0, 1.0f, -1.0f);
				lightDirection = lightDirection.normalize();
				float dp = std::max(0.1f, normal.dot(lightDirection));

				triProj *= m_camera->GetView();

				int nClippedTriangles = 0;
				Triangle clipped[2];
				nClippedTriangles = ClipAgainstPlane({ 0.0f, 0.0f, 2.0f }, { 0.0f, 0.0f, 1.0f }, triProj, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++)
				{
					for (Vector3& vertex : clipped[n].vertices) {
						vertex = m_camera->GetProjection() * vertex;
						vertex = vertex / vertex.w;
						vertex.x += 1.0f; vertex.y += 1.0f;
						vertex.x *= 0.5f * (float)ScreenWidth(); vertex.y *= 0.5f * (float)ScreenHeight();
					}

					clipped[n].color = triangle.color * dp;
					m_sumtriangles.push_back(clipped[n]);
				}
			}
		}

	}

	void CupEngine::DrawCupTriangle(const Triangle& triangle, const Vector4& color)
	{
		FillTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(color.x, color.y, color.z, 255.0f));
		//DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(0.0f, 0.0f, 0.0f, 255.0f));
	}

	float CupEngine::GetAspectRatio() const
	{
		return (float)ScreenHeight() / (float)ScreenWidth();
	}
}