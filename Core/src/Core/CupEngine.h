#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include <functional>

#include "Math/CupMath.h"
#include "LayerStack.h"

#include "Olc/olcPixelGameEngine.h"

namespace Cup {

	class Mesh
	{
    public:
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
	};

	class CupEngine : public olc::PixelGameEngine
	{
    public:
        static CupEngine& Instance() { return *s_instance; }
	public:
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

        inline float GetAspectRatio() const;
	private:
		void DrawCupTriangle(const Triangle& triangle, const Vector4& color);

        LayerStack m_layerstack;

		float m_theta = -36.0f;
        float m_yaw = 0.0f;
		Mesh m_cube;

        Vector3 m_camPos;
		CameraProps m_cameraProps;

        Vector3 m_camera;
        Vector3 m_lookDir = Vector3(0, 0, 1);

		Matrix4x4 m_projectionMatrix;
        Matrix4x4 m_viewMatrix;

        static CupEngine* s_instance;
	};

}