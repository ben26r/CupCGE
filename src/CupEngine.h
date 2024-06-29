#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include "CupMath.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

namespace Cup {


	struct Triangle
	{
		Vector3 vertices[3];
		Vector4 color;

		Vector3& operator[](int index) { return vertices[index]; }
		const Vector3& operator[](int index) const { return vertices[index]; }
	};

	struct Mesh
	{
		std::vector<Triangle> triangles;

        bool LoadModel(const std::string& filepath)
        {
            std::ifstream file(filepath);
            if (!file.is_open())
                return false;

            std::vector<Vector3> vertices;
            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::string prefix;
                iss >> prefix;

                if (prefix == "v")
                {
                    Vector3 vertex;
                    iss >> vertex.x >> vertex.y >> vertex.z;
                    vertices.push_back(vertex);
                }
                else if (prefix == "f")
                {
                    Triangle triangle;
                    int vertexIndices[3];
                    for (int i = 0; i < 3; ++i)
                    {
                        iss >> vertexIndices[i];
                        // OBJ files are 1-indexed
                        triangle[i] = vertices[vertexIndices[i] - 1];
                    }
                    triangles.push_back(triangle);
                }
            }

            file.close();
            return true;
        }
	};

	struct CameraProps
	{
		float cnear = 0.1f;
		float cfar = 1000.0f;
		float fov = 90.0f;
		float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

		void SetFov(float Fov) { fov = Fov; fovRad = 1.0f / tanf(Fov * 0.5f / 180.0f * 3.14159f); }
	};

	class CupEngine : public olc::PixelGameEngine
	{
	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
	private:
		void DrawCupTriangle(const Triangle& triangle, const olc::Pixel color);

		float m_theta = -90.0f;
		Mesh m_cube;
		CameraProps m_cameraProps;
		Matrix4x4 m_projectionMatrix;
	};

}