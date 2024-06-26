#pragma once
#include <vector>

#include "CupMath.h"
#include "olcConsoleGameEngine.h"

namespace Cup {


	struct Triangle
	{
		Vector3 vertices[3];

		Vector3& operator[](int index) { return vertices[index]; }
		const Vector3& operator[](int index) const { return vertices[index]; }
	};

	struct Mesh
	{
		std::vector<Triangle> triangles;
	};

	struct CameraProps
	{
		float cnear = 0.1f;
		float cfar = 1000.0f;
		float fov = 90.0f;
		float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

		void SetFov(float Fov) { fov = Fov; fovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f); }
	};

	class CupEngine : public olcConsoleGameEngine
	{
	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
	private:
		void DrawCupTriangle(const Triangle& triangle, short c, short color);

		float m_theta = -90.0f;
		Mesh m_cube;
		CameraProps m_cameraProps;
		Matrix4x4 m_projectionMatrix;
	};

}