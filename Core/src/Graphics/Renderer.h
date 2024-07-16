#pragma once

#include <algorithm>
#include <list>
#include <functional>

#include "Core/Base.h"
#include "Camera.h"
#include "Math/CupMath.h"
#include "Olc/olcPixelGameEngine.h"

namespace Cup {

	class Renderer
	{
	public:

		static bool Init(olc::PixelGameEngine* appPtr);

		static void Submit(const std::shared_ptr<Camera>& camera, const Matrix4x4f& matrix, const Meshf& mesh);

        static void Sort();
		static void Start();
        static void End();

        static void DrawCupTriangle(const Trianglef& triangle, const Vector4& color);
		//static void Renderer::DrawString(const std::string& text, const Vector4& color);
	private:
		struct RendererData
		{
			std::vector<Trianglef> sumTriangles;
            olc::PixelGameEngine* appPtr = nullptr;
		};

		static RendererData m_rendererData;
	};
}