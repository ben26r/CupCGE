#include "CupEngine.h"

namespace Cup {

	bool CupEngine::OnUserCreate()
	{
		m_cube.triangles = {
			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

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
		
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

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

		for (auto& triangle : m_cube.triangles)
		{
			Triangle triProj;
			triProj[0] = MultiplyVectorMatrix(triangle[0], matRotZ);
			triProj[1] = MultiplyVectorMatrix(triangle[1], matRotZ);
			triProj[2] = MultiplyVectorMatrix(triangle[2], matRotZ);

			triProj[0] = MultiplyVectorMatrix(triProj[0], matRotX);
			triProj[1] = MultiplyVectorMatrix(triProj[1], matRotX);
			triProj[2] = MultiplyVectorMatrix(triProj[2], matRotX);

			triProj[0].z += 3.0f;
			triProj[1].z += 3.0f;
			triProj[2].z += 3.0f;

			triProj[0] = MultiplyVectorMatrix(triProj[0], m_projectionMatrix);
			triProj[1] = MultiplyVectorMatrix(triProj[1], m_projectionMatrix);
			triProj[2] = MultiplyVectorMatrix(triProj[2], m_projectionMatrix);

			// Scale into view
			triProj[0].x += 1.0f; triProj[0].y += 1.0f;
			triProj[1].x += 1.0f; triProj[1].y += 1.0f;
			triProj[2].x += 1.0f; triProj[2].y += 1.0f;
			triProj[0].x *= 0.5f * (float)ScreenWidth();
			triProj[0].y *= 0.5f * (float)ScreenHeight();
			triProj[1].x *= 0.5f * (float)ScreenWidth();
			triProj[1].y *= 0.5f * (float)ScreenHeight();
			triProj[2].x *= 0.5f * (float)ScreenWidth();
			triProj[2].y *= 0.5f * (float)ScreenHeight();


			DrawCupTriangle(triProj, PIXEL_SOLID, FG_WHITE);
		}

		return true;
	}

	void CupEngine::DrawCupTriangle(const Triangle& triangle, short c, short color)
	{
		DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, c, color);
	}
}


int main()
{
	Cup::CupEngine demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();

	return 0;
}