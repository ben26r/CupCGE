#include "CupEngine.h"
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
		if (!m_cube.LoadModel("src/assets/VideoShip.obj"))
		{

		}

		for (auto& tri : m_cube.triangles)
		{
			tri.color = Vector4(155, 155, 155, 255);
		}

		float aspectRatio = (float)ScreenHeight() / (float)ScreenWidth();

		m_projectionMatrix[0][0] = aspectRatio * m_cameraProps.fovRad;
		m_projectionMatrix[1][1] = m_cameraProps.fovRad;
		m_projectionMatrix[2][2] = m_cameraProps.cfar / (m_cameraProps.cfar - m_cameraProps.cnear);
		m_projectionMatrix[3][2] = (-m_cameraProps.cfar * m_cameraProps.cnear) / (m_cameraProps.cfar - m_cameraProps.cnear);
		m_projectionMatrix[2][3] = 1.0f;
		m_projectionMatrix[3][3] = 0.0f;

		return true;
	}

	bool CupEngine::OnUserUpdate(float fElapsedTime)
	{
		
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));

		Matrix4x4 matRotZ, matRotX;
		m_theta += 1.0f * fElapsedTime;

		// Rotation Z
		matRotZ[0][0] = cosf(m_theta);
		matRotZ[0][1] = sinf(m_theta);
		matRotZ[1][0] = -sinf(m_theta);
		matRotZ[1][1] = cosf(m_theta);
		matRotZ[2][2] = 1;
		matRotZ[3][3] = 1;

		// Rotation X
		matRotX[0][0] = 1;
		matRotX[1][1] = cosf(m_theta * 0.5f);
		matRotX[1][2] = sinf(m_theta * 0.5f);
		matRotX[2][1] = -sinf(m_theta * 0.5f);
		matRotX[2][2] = cosf(m_theta * 0.5f);
		matRotX[3][3] = 1;

		std::vector<Triangle> sumtriangles;

		for (const auto& triangle : m_cube.triangles)
		{
			Triangle triProj;
			triProj[0] = MultiplyVectorMatrix(triangle[0], matRotZ);
			triProj[1] = MultiplyVectorMatrix(triangle[1], matRotZ);
			triProj[2] = MultiplyVectorMatrix(triangle[2], matRotZ);

			MultiplyVectorMatrix(triProj[0], matRotX);
			MultiplyVectorMatrix(triProj[1], matRotX);
			MultiplyVectorMatrix(triProj[2], matRotX);

			triProj[0].z += 8.0f;
			triProj[1].z += 8.0f;
			triProj[2].z += 8.0f;

			Vector3 normal, line1, line2;
			line1 = triProj[1] - triProj[0];
			line2 = triProj[2] - triProj[0];
			normal = line1.cross(line2);

			normal = normal.normalize();

			if (normal.dot(triProj[0]) < 0.0f)
			{
				Vector3 lightDirection = Vector3(0, 0, -1);
				lightDirection = lightDirection.normalize();
				float dp = normal.dot(lightDirection);

				MultiplyVectorMatrix(triProj[0], m_projectionMatrix);
				MultiplyVectorMatrix(triProj[1], m_projectionMatrix);
				MultiplyVectorMatrix(triProj[2], m_projectionMatrix);

				triProj[0].x += 1.0f; triProj[0].y += 1.0f;
				triProj[1].x += 1.0f; triProj[1].y += 1.0f;
				triProj[2].x += 1.0f; triProj[2].y += 1.0f;
				triProj[0].x *= 0.5f * (float)ScreenWidth();
				triProj[0].y *= 0.5f * (float)ScreenHeight();
				triProj[1].x *= 0.5f * (float)ScreenWidth();
				triProj[1].y *= 0.5f * (float)ScreenHeight();
				triProj[2].x *= 0.5f * (float)ScreenWidth();
				triProj[2].y *= 0.5f * (float)ScreenHeight();

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