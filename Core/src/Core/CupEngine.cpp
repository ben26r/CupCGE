#include "Core/CupEngine.h"
#include <algorithm>
#include <list>

namespace Cup {

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
		if (!m_cube.LoadModel("assets/axis.obj"))
		{

		}

		for (auto& tri : m_cube.triangles)
		{
			tri.color = Vector4(155, 155, 155, 255);
		}

		float aspectRatio = (float)ScreenHeight() / (float)ScreenWidth();

		m_projectionMatrix = Matrix4x4::Projection(m_cameraProps.fov, aspectRatio, m_cameraProps.cnear, m_cameraProps.cfar);

		return true;
	}

	bool CupEngine::OnUserUpdate(float fElapsedTime)
	{
		
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));

		std::vector<Triangle> sumtriangles;

		float rotx = cosf(m_theta * 0.5f);
		float roty = sinf(m_theta * 0.5f);
		//m_theta += 1.0f * fElapsedTime;

		if (GetKey(olc::Key::LEFT).bHeld) m_camPos.x -= 10.0f * fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) m_camPos.x += 10.0f * fElapsedTime;
		if (GetKey(olc::Key::UP).bHeld) m_camPos.y -= 10.0f * fElapsedTime;
		if (GetKey(olc::Key::DOWN).bHeld) m_camPos.y += 10.0f * fElapsedTime;


		Vector3 foward = m_lookDir * 10 * fElapsedTime;

		if (GetKey(olc::Key::S).bHeld)
			m_camPos = m_camPos - foward;

		if (GetKey(olc::Key::W).bHeld)
			m_camPos = m_camPos + foward;

		if (GetKey(olc::Key::A).bHeld)
			m_yaw += 2.0f * fElapsedTime;

		if (GetKey(olc::Key::D).bHeld)
			m_yaw -= 2.0f * fElapsedTime;

		for (const auto& triangle : m_cube.triangles)
		{
			Triangle triProj = triangle;
			for (Vector3& vertex : triProj.vertices) {
				vertex = PreRotate(vertex, Vector3(1, 1, 0), m_theta, rotx, roty); }

			m_lookDir = MultiplyVectorMatrix(Vector3::Far(), Matrix4x4::Rotation(Vector3::Up(), m_yaw));
			Vector3 target = m_camPos + m_lookDir;
			m_viewMatrix = Matrix4x4::PointAt(m_camPos, target, Vector3::Up()).QuickInverse();

			triProj.Do([](Vector3& vertex) { vertex.z += 8.0f; });
			Vector3 normal = (triProj[1] - triProj[0]).cross((triProj[2] - triProj[0])).normalize();

			if (normal.dot(triProj[0] - m_camPos) < 0.0f)
			{
				Vector3 lightDirection = Vector3(0, 0, -1);
				lightDirection = lightDirection.normalize();
				float dp = normal.dot(lightDirection);

				MultiplyVectorMatrix(triProj[0], m_viewMatrix);
				MultiplyVectorMatrix(triProj[1], m_viewMatrix);
				MultiplyVectorMatrix(triProj[2], m_viewMatrix);

				int nClippedTriangles = 0;
				Triangle clipped[2];
				nClippedTriangles = ClipAgainstPlane({ 0.0f, 0.0f, 2.0f }, { 0.0f, 0.0f, 1.0f }, triProj, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++)
				{
					for (Vector3& vertex : clipped[n].vertices) {
						MultiplyVectorMatrix(vertex, m_projectionMatrix);
						vertex = vertex / vertex.w;
						vertex.x += 1.0f; vertex.y += 1.0f;
						vertex.x *= 0.5f * (float)ScreenWidth(); vertex.y *= 0.5f * (float)ScreenHeight();
					}

					clipped[n].color = triangle.color * dp;
					sumtriangles.push_back(clipped[n]);
				}
			}
		}

		std::sort(sumtriangles.begin(), sumtriangles.end(), [](const Triangle& triangle, const Triangle& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});

		for (const Triangle& triangle : sumtriangles)
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
				DrawCupTriangle(t, olc::Pixel(t.color.x, t.color.y, t.color.z));
			}
		}

		return true;
	}

	void CupEngine::DrawCupTriangle(const Triangle& triangle, const olc::Pixel color)
	{
		FillTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, color);
	}
}

int main()
{
	Cup::CupEngine demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}