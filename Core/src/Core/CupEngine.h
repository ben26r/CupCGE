#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include <functional>
#include <memory>

#include "Math/CupMath.h"
#include "Graphics/Camera.h"
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

	class CupEngine : public olc::PixelGameEngine
	{
    public:
        static CupEngine& Instance() { return *s_instance; }
	public:
        CupEngine();
        ~CupEngine() = default;

		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

        void Submit(const Mesh& mesh);
        void PushLayer(Layer* layer) { m_layerstack.PushLayer(layer); layer->OnAttach(); }

        inline float GetAspectRatio() const;
	private:
		void DrawCupTriangle(const Triangle& triangle, const Vector4& color);

        LayerStack m_layerstack;

        std::shared_ptr<Camera> m_camera;
        std::vector<Triangle> m_sumtriangles;

        static CupEngine* s_instance;
	};

    CupEngine* CreateApplication();
}