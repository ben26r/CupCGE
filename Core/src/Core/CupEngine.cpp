#include "Core/CupEngine.h"
#include <algorithm>

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

				for (Vector3& vertex : triProj.vertices) {
					MultiplyVectorMatrix(vertex, m_viewMatrix);
					MultiplyVectorMatrix(vertex, m_projectionMatrix);
					vertex = vertex / vertex.w;
					vertex.x += 1.0f; vertex.y += 1.0f;
					vertex.x *= 0.5f * (float)ScreenWidth(); vertex.y *= 0.5f * (float)ScreenHeight();
				}

				triProj.color = triangle.color * dp;
				sumtriangles.push_back(triProj);
			}
		}

		std::sort(sumtriangles.begin(), sumtriangles.end(), [](const Triangle& triangle, const Triangle& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});

		for (const Triangle& triangle : sumtriangles)
		{
			DrawCupTriangle(triangle, olc::Pixel(triangle.color.x, triangle.color.y, triangle.color.z));
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